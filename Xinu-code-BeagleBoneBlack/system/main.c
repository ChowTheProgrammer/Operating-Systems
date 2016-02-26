/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>
int32 cpudelay;
volatile uint32	gcounter = 400000000;
process	counterproc(ppid32 pipe) {

	/*while(gcounter > 0) {*/
		/*gcounter--;*/
	/*}*/
    sleep(1);
    char c = 'A';
    pjoin(pipe);
    sleep(1);
    pread(pipe, &c, 1);
    kprintf("reader: I got %c. \r\n", c);
    pclose(pipe);
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
    char c = 'B';
    ppid32 pipe = popen("w");
    resume(create(counterproc, 4096, 20, "counterproc", 1, pipe));
    sleep(1);
    pwrite(pipe, &c, 1);
    kprintf("main: I sent %c. \r\n", c);
    sleep(3);
    pclose(pipe);

	return OK;
}
