/* user.c : User processes
* 
* name:		Jack Wu
* student id:	17254079
*/

#include <xeroskernel.h>

/* Your code goes here */



/*
* root
*
* @desc:	executes the root process
*/
void idleproc (void)
{
	for(;;);
}

/*
* root
*
* @desc:	executes the root process
*/
void root()
{

//	syscreate(&producer, PROC_STACK);
//	//syscreate(&consumer, PROC_STACK);

	/* eventual loop after producer and consumer exits */
	for(;;);
}

/*
* producer
*
* @desc:	executes the producer process
*/
void producer ()
{
	int i;

	/*
	* Assignment 1: 	cycle 12 iterations 
	*			print "Happy ", sysyield()	
	*/

	syssleep(101);
/*
	for(i=0; i<12; i++)
		kprintf("producer\n");
*/
}

/*
* consumer
*
* @desc:	executes the consumer process
*/
void consumer ()
{
	int i;

	/*
	* Assignment 1: 	cycle 15 iterations 
	*			print "New Year", sysyield()	
	*/
	for(i=0; i<15; i++) 
		kprintf("consumer\n");
}
