/* pipe.c - popen, pjoin, pclose, pread, pwrite */

#include <xinu.h>

int32   pipcount;

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
    pipcount++;

    /* TODO: May need to set mem here */
    piptab[pip].pipmode = mode[0]; /* Initialize mode for the pip entry */

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
            piptab[pip].pipstate = PIPE_USED;
            piptab[pip].pipparent = (pid32)getpid();
        kprintf("popen: The owner for pipe:%d is %d. \r\n", i, piptab[pip].pipparent);
            return pip;
        }
    }
    return SYSERR;
}
