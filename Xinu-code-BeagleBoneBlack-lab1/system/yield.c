/* yield.c - yield */

#include <xinu.h>

int     resche_caller;
/*------------------------------------------------------------------------
 *  yield  -  Voluntarily relinquish the CPU (end a timeslice)
 *------------------------------------------------------------------------
 */
syscall	yield(void)
{
	intmask	mask;			/* Saved interrupt mask		*/

	mask = disable();
    resche_caller = YIELD_RESCHE;
	resched();
	restore(mask);
	return OK;
}
