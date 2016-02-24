/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>
int32 cpudelay;
volatile uint32	gcounter = 400000000;
process	counterproc() {

	while(gcounter > 0) {
		gcounter--;
	}
	return OK;
}

process	main(void)
{
	/* Start the network */

	/*netstart();*/

	/*kprintf("\n...creating a shell\n");*/
	/*recvclr();*/
	/*resume(create(shell, 8192, 50, "shell", 1, CONSOLE));*/

	/*[> Wait for shell to exit and recreate it <]*/

	/*while (TRUE) {*/
		/*receive();*/
		/*sleepms(200);*/
		/*kprintf("\n\nMain process recreating shell\n\n");*/
		/*resume(create(shell, 4096, 20, "shell", 1, CONSOLE));*/
	/*}*/
    kprintf("main: CURRENT PID is: %d. \r\n", (int32)getpid());

    ppid32 pipe1 = popen("w");
    ppid32 pipe2 = popen("r");
    ppid32 pipe3 = popen("x");

    kprintf("main: Pipe id: %d. \r\n", pipe1);
    kprintf("main: pipe1's reader pid is: %d. \r\n", piptab[pipe1].pipreader);
    kprintf("main: pipe1's writer pid is: %d. \r\n", piptab[pipe1].pipwriter);

    kprintf("main: Pipe id: %d. \r\n", pipe2);
    kprintf("main: pipe2's reader pid is: %d. \r\n", piptab[pipe2].pipreader);
    kprintf("main: pipe2's writer pid is: %d. \r\n", piptab[pipe2].pipwriter);

    kprintf("main: Pipe id: %d. \r\n", pipe3);

    pjoin(pipe1);
    kprintf("main: pipe1's reader pid is: %d. \r\n", piptab[pipe1].pipreader);

	return OK;
}
