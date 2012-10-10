/* ctsw.c : context switcher
* 
* name:		Jack Wu
* student id:	17254079
*/

#include <xeroskernel.h>
#include <i386.h>

/* Your code goes here - You will need to write some assembly code. You must
   use the gnu conventions for specifying the instructions. (i.e this is the
   format used in class and on the slides.) You are not allowed to change the
   compiler/assembler options or issue directives to permit usage of Intel's
   assembly language conventions.
*/


void _ISREntryPoint(void);		/* kernel interrupt routine */

static unsigned int esp;		/* user stack pointer location */
static unsigned int k_esp;		/* kernel stack pointer location */
static unsigned int rc;			/* syscall() call request id */
static unsigned int args;		/* args passed from syscall() */

/*
* contextswitch
*
* @desc:	entrant/exit point between kernel and application
*
* @param:	p	process pcb block to context switch into
*
* @output:	rc	system call request id
*/
int contextswitch( pcb_t *p ) 
{
	k_esp=0;

	/* save process esp */
	esp=p->esp;	

	/*
	* context switch between process and kernel
	* retrieve syscall() arguments by register from 'eax' and 'edx'
	*/
	__asm __volatile( " \
     		pushf  \n\
     		pusha  \n\
     		movl  %%esp, k_esp     \n\
     		movl  esp, %%esp  \n\
     		popa  \n\
		iret \n\
	_ISREntryPoint:  \n\
    		pusha   \n\
    		movl  %%esp, esp \n\
    		movl  k_esp, %%esp \n\
		movl %%eax, rc \n\
		movl %%edx, args \n\
    		popa \n\
    		popf \n\
        	"
  		:
  		: 
  		: "%eax"
  		);
 
	/* save process esp and passed args */
	p->esp = esp;
	p->args = args;

	return rc;
}

/*
* contextinit
*
* @desc:	set kernel interrupt event entry in IDT
*/
void contextinit() 
{
	set_evec(KERNEL_INT, _ISREntryPoint);
}

