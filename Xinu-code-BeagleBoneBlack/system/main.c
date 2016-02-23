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

    ppid32 pipe1 = popen("w");
    ppid32 pipe2 = popen("r");

    kprintf("main: I created a pipe with pid: %d. \r\n", pipe1);
    kprintf("main: I created a pipe with pid: %d. \r\n", pipe2);

	return OK;
}
