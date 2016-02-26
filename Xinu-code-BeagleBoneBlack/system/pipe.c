/* pipe.c - popen, pjoin, pclose, pread, pwrite */

#include <xinu.h>


local   ppid32    newpip(void);

/* Pipe creation */

ppid32 popen(const char *mode) {
    intmask     mask;
    ppid32      pip;

    mask = disable();

    kprintf("popen: The mode is %c \r\n", mode[0]);

    if ((mode[0] != 'r' && mode[0] != 'w') || ((pip=newpip())==SYSERR)) {
        restore(mask);
        return SYSERR;
    }

    struct pipent *pipptr;

    pipptr = &piptab[pip];


    pipptr->pipmode = mode[0]; /* Initialize mode for the pip entry */
    if (mode[0]=='r' && pipptr->pipreader==END_VALID) {
        pipptr->pipreader = (int32)getpid();
    }
    else if (mode[0]=='w' && pipptr->pipwriter==END_VALID) {
        pipptr->pipwriter = (int32)getpid();
    }
    else {
        restore(mask);
        return SYSERR;
    }

    pipptr->pipsem = semcreate(1); /* Create sem for the pipe */

    restore(mask);
    return pip;
}

local   ppid32  newpip(void)
{
    static  ppid32  nextpip = 0;    /* Next pipe index to try */
    ppid32  pip;    /*  Pipe ID to return   */
    int32   i;      /* Iterate through # entries    */

    for (i=0; i<NPIPE; i++) {
        pip = nextpip++;

        if (nextpip >= NPIPE) {
            nextpip = 0;
        }
        if (piptab[pip].pipstate == PIPE_FREE) {
            piptab[pip].pipstate = PIPE_OPENED;
            piptab[pip].pipparent = (pid32)getpid();
        /*kprintf("popen: The owner for pipe:%d is %d. \r\n", i, piptab[pip].pipparent);*/
            return pip;
        }
    }
    return SYSERR;
}

syscall pjoin   (ppid32 pipeid) {
    intmask     mask;

    mask = disable();
    kprintf("pjoin: Pipe try to join is %d. \r\n", pipeid);

    if (isbadppid(pipeid) || piptab[pipeid].pipstate!=PIPE_OPENED) {
        restore(mask);
        return SYSERR;
    }

    struct pipent *pipptr;

    pipptr = &piptab[pipeid];

    if (pipptr->pipmode == 'w')
        pipptr->pipreader = (int32)getpid();
    else
        pipptr->pipwriter = (int32)getpid();

    pipptr->pipstate = PIPE_JOINED;      /* change the state to JOINED */

    kprintf("pjoin: pid %d join %d successfully. \r\n", getpid(), pipeid);

    return OK;
}


syscall pclose  (ppid32 pipeid) {
    intmask     mask;

    mask = disable();

    if (isbadppid(pipeid)) {
        restore(mask);
        return SYSERR;
    }

    struct pipent *pipptr;

    pipptr = &piptab[pipeid];

    if (pipptr->pipreader == (int32)getpid()) { /* close reader end */
        pipptr->pipreader = END_VALID;
    }
    else if (pipptr->pipwriter == (int32)getpid()) { /* close writer end */
        pipptr->pipwriter = END_VALID;
    }
    else {
        restore(mask);
        return SYSERR;
    }

    if (semcount(pipptr->pipsem) < 0) { /* Wake up the blocked process by signal */
        signal(pipptr->pipsem);
    }

    if (pipptr->pipreader==END_VALID && pipptr->pipwriter==END_VALID) {
        /* Free pipe if both ends are closed */
        /* Then initialize all the fields */

        pipptr->pipstate = PIPE_FREE;
        pipptr->pipparent = -1;
        pipptr->pipsem = semdelete(pipptr->pipsem);
        freemem(pipptr->buffer, PIPESIZE);
        pipptr->pipbufs = 0;
        pipptr->pipbufc = 0;
        pipptr->pipmode = '\0';
    }

    return OK;
}


syscall pread(ppid32 pipeid, void *buf, uint32 len) {
    intmask     mask;
    mask = disable();

    if (isbadppid(pipeid)) {
        restore(mask);
        return SYSERR;
    }

    struct pipent *pipptr;

    pipptr = &piptab[pipeid];

    if (pipptr->pipstate!=PIPE_JOINED || pipptr->pipreader!=(int32)getpid()) {
        /* Check pipestate and process permission to read */
        restore(mask);
        return SYSERR;
    }

    byte *buffer = buf;
    int counter = 0;
    byte b;

    wait(pipptr->pipsem);

    while(counter<len) {

        if (pipptr->pipbufc == 0) { /* No data in the pipe */
            if(pipptr->pipwriter == END_VALID) { /* Write end is closed */
                kprintf("pread: no data and write end is closed. \r\n");
                signal(pipptr->pipsem);
                restore(mask);
                return SYSERR;
            }
            else {
                kprintf("pread: no data but waiting for writer. \r\n");
                signal(pipptr->pipsem);
                resched();
                wait(pipptr->pipsem);
            }
        }
        b = pipptr->buffer[pipptr->pipbufs];
        if (b=='\0') {
            break;
        }
        *buffer++ = b;
        pipptr->pipbufc--;
        pipptr->pipbufs = (pipptr->pipbufs + 1) % PIPESIZE;
        counter++;
    }

    kprintf("pread: read complete. Total bytes read is %d. \r\n", counter);
    signal(pipptr->pipsem);
    restore(mask);
    return counter;
}


syscall pwrite(ppid32 pipeid, void *buf, uint32 len) {
    intmask     mask;
    mask = disable();

    if (isbadppid(pipeid)) {
        restore(mask);
        return SYSERR;
    }

    struct pipent *pipptr;

    pipptr = &piptab[pipeid];

    if (pipptr->pipstate!=PIPE_JOINED || pipptr->pipwriter!=(int32)getpid()) {
        restore(mask);
        return SYSERR;
    }

    byte *buffer = buf;
    int counter = 0;

    wait(pipptr->pipsem);

    while(counter<len) {

        if(pipptr->pipreader == END_VALID) {
            kprintf("pwrite: no reader. \r\n");
            signal(pipptr->pipsem);
            restore(mask);
            return SYSERR;
        }
        if(pipptr->pipbufc == PIPESIZE) {
            kprintf("pwrite: no free space for writer. \r\n");
            signal(pipptr->pipsem);
            resched();
            wait(pipptr->pipsem);
        }


        pipptr->buffer[(pipptr->pipbufs + pipptr->pipbufc) % PIPESIZE] = *buffer++;
        counter++;
        pipptr->pipbufc++;
    }

    kprintf("pwrite: write complete. Total bytes wrote is %d. \r\n", counter);
    signal(pipptr->pipsem);
    restore(mask);
    return counter;
}
