/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>
#include <lab0.h>
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
    printf("Hello world!\n");

    resume (
        create((void *)lab0app1, INITSTK, INITPRIO, "Lab0app1", 1,
            NULL));
    resume (
        create((void *)lab0app2, INITSTK, INITPRIO, "Lab0app2", 2,
            NULL));


	return OK;
}
