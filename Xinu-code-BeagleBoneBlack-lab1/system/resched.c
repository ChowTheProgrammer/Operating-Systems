/* resched.c - resched, resched_cntl */

#include <xinu.h>

struct	defer	Defer;

int     resche_caller;

pid32   prioque[NQUE];

/*------------------------------------------------------------------------
 *  resched  -  Reschedule processor to highest priority eligible process
 *------------------------------------------------------------------------
 */
void	resched(void)		/* Assumes interrupts are disabled	*/
{
	struct procent *ptold;	/* Ptr to table entry for old process	*/
	struct procent *ptnew;	/* Ptr to table entry for new process	*/

	/* If rescheduling is deferred, record attempt and return */

	if (Defer.ndefers > 0) {
		Defer.attempt = TRUE;
		return;
	}

	/* Point to process table entry for the current (old) process */

	ptold = &proctab[currpid];


    if (preempt < QUANTUM && (resche_caller == SLEEP_RESCHE ||
                resche_caller == RECV_RESCHE ||
                resche_caller == YIELD_RESCHE)) {   /* Classify process type */
        if (ptold->pr_class == PRCLS_IOB) {
            ptold->prprio+=2;
        }
        ptold->pr_class = PRCLS_IOB;
        ptold->prprio++;
    }
    else {
        if (ptold->pr_class == PRCLS_CPUB) {
            ptold->prprio-=2;
        }
        ptold->pr_class = PRCLS_CPUB;
        ptold->prprio--;
    }

    if (ptold->prprio >= 497 && ptold->prprio <= 503) {
        ptold->prprio = 500;    // Processes with priority 500 will be fixed
    }
    else if(ptold->prprio > 8) {
        ptold->prprio = 8;
    }
    else if(ptold->prprio < 0) {
        ptold->prprio = 0;
    }

	if (ptold->prstate == PR_CURR) {  /* Process remains eligible */
		if (ptold->prprio > firstkey(readylist)) {
			return;
		}


		/* Old process will no longer remain current */

		ptold->prstate = PR_READY;
		/*insert(currpid, readylist, ptold->prprio);*/

        /* Insert process between curr and next node */

        int16   prev;
        int16   next;


        /* Get the pid of last process with the same prio as ptold */

        if (ptold->prprio == 500) {
            prev = prioque[9];
        }
        else {
            prev = prioque[ptold->prprio];
        }

        if (prev >= 0) { // update the queuetab in O(1) time
            next = queuetab[prev].qnext;
            queuetab[currpid].qnext = next;
            queuetab[currpid].qprev = prev;
            queuetab[currpid].qkey = ptold->prprio;
            queuetab[prev].qnext = currpid;
            queuetab[next].qprev = currpid;
        }

        /* Update prioque by replacing currpid to the priority array */
        if (ptold->prprio == 500) {
            prioque[9] = currpid;
        }
        else {
            prioque[ptold->prprio] = currpid;
        }
	}

	/* Force context switch to highest priority ready process */

	currpid = dequeue(readylist);
    if (currpid == 0 && !isempty(readylist)) {     // Null process
        insert(currpid, readylist, 0);
        currpid = dequeue(readylist);
    }
	ptnew = &proctab[currpid];
	ptnew->prstate = PR_CURR;
	/*preempt = QUANTUM;		 // Reset time slice for process   */

    /* Set a new quantum time according to its priority level*/

    if (ptnew->prprio >= 0 && ptnew->prprio <= 2) {
        preempt = 200 - (ptnew->prprio)*20;
    }
    else if (ptnew->prprio >= 3 && ptnew->prprio <= 8) {
        preempt = 180 - (ptnew->prprio)*20;
    }
    else { /* priority is 500 */
        preempt = 10;
    }

    if (currpid == 0) {
        return;
    }
	ctxsw(&ptold->prstkptr, &ptnew->prstkptr);

	/* Old process returns here when resumed */

	return;
}

/*------------------------------------------------------------------------
 *  resched_cntl  -  Control whether rescheduling is deferred or allowed
 *------------------------------------------------------------------------
 */
status	resched_cntl(		/* Assumes interrupts are disabled	*/
	  int32	defer		/* Either DEFER_START or DEFER_STOP	*/
	)
{
	switch (defer) {

	    case DEFER_START:	/* Handle a deferral request */

		if (Defer.ndefers++ == 0) {
			Defer.attempt = FALSE;
		}
		return OK;

	    case DEFER_STOP:	/* Handle end of deferral */
		if (Defer.ndefers <= 0) {
			return SYSERR;
		}
		if ( (--Defer.ndefers == 0) && Defer.attempt ) {
			resched();
		}
		return OK;

	    default:
		return SYSERR;
	}
}
