/* resched.h */

/* Constants and variables related to deferred rescheduling */

#define	DEFER_START	1	/* Start deferred rescehduling		*/
#define	DEFER_STOP	2	/* Stop  deferred rescehduling		*/

/* Structure that collects items related to deferred rescheduling	*/

struct	defer	{
	int32	ndefers;	/* Number of outstanding defers 	*/
	bool8	attempt;	/* Was resched called during the	*/
				/*   deferral period?			*/
};

extern	struct	defer	Defer;

extern  int     resche_caller;


#define READY_RESCHE 0
#define RESCH_RESCHE 1
#define WAIT_RESCHE  2
#define YIELD_RESCHE 3
#define SYSPD_RESCHE 4
#define RECVT_RESCHE 5
#define RECV_RESCHE  6
#define CLKHD_RESCHE 7
#define SLEEP_RESCHE 8
#define KILL_RESCHE  9
