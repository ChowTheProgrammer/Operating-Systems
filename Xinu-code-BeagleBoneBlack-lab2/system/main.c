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
    char array[4] = "ab\0c";
    pjoin(pipe);
    sleep(1);
    pread(pipe, &array, 4);
    kprintf("reader: I got %d,%d,%d,%d. \r\n", array[0], array[1], array[2], array[3]);
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
    char array[4] = "d\0ef";
    ppid32 pipe = popen("w");
    resume(create(counterproc, 4096, 20, "counterproc", 1, pipe));
    sleep(1);
    pwrite(pipe, &array, 4);
    kprintf("main: I sent 100,0,101,102. \r\n");
    sleep(3);
    pclose(pipe);

	return OK;
}
