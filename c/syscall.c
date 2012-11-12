/* syscall.c : syscalls
* 
* name:		Jack Wu
* student id:	17254079
*/

#include <xeroskernel.h>
#include <stdarg.h>

/* Your code goes here */
static unsigned int rc;

/*
* syscall
*
* @desc:	generates an interrupt call to the kernel
*
* @param:	call		system call request id
*		funcptr		process function
*		stack		process stack size
*
* @output:	rc		return value of the kernel function for the request id
*/
int syscall (int call, ...) {
    	va_list	ap;
	va_start(ap, call);

	/* pass syscall() args by register, 'eax' and 'edx' */
	/* after the context has been switched back to the process stack, the return value is retrieved from eax */
	__asm __volatile( " \
		movl %0, %%eax 	\n\
		movl %1, %%edx 	\n\
		int %2		\n\
		movl %%eax, rc 	\n\
     		"
		:
		: "r"(call), "r"(ap), "i" (KERNEL_INT)
	);
	
	return rc;
}

/*
* syscreate
*
* @desc:	signals a create process interrupt 
*
* @param:	funcptr		process function
*		stack		process stack size
*
* @output:	rc		return status of the kernel create() function
*/
int syscreate(void (*func)(void), int stack)
{
	return syscall(CREATE, func, stack);
}

/*
* sysyield
*
* @desc:	signals a yield process interrupt
*/
void sysyield() 
{
	syscall(YIELD);
}

/*
* sysstop
*
* @desc:	signals a stop process interrupt
*/
void sysstop()
{
	syscall(STOP);
}

/*
* sysgetpid
*
* @desc:	returns the current process pid
* 
* @output:	pid		pid of the calling proc
*/
unsigned int sysgetpid(void)
{
	return syscall(GETPID);
}

/*
* sysputs
*
* @desc:	signals a synchronous kernel console output message
*
* @param:	str		string message to display on console
*/
void sysputs( char *str )
{
	syscall(PUTS, str);
}

/*
* syssleep
*
* @desc:	signals the current process to sleep for input milliseconds
*
* @param:	milliseconds		time in milliseconds for a process to sleep
* 
* @output:
*/
unsigned int syssleep( unsigned int milliseconds )
{
	return syscall(SLEEP, milliseconds);
}

/*
* syssend
*
* @desc:	signals a ipc_send for the current process
*
* @param:	dest_pid	string message to display on console
*		buffer		buffer for sending the ipc message
*		buffer_len	length of the send buffer
*		
* @output:	rc		returns the number of bytes that have been received, in exceptional cases, the following will be returned
*				-1	invalid pid
*				-2	loopback pid
*				-3 	other ipc errors	
*/
int syssend(unsigned int dest_pid, void *buffer, int buffer_len)
{
	return syscall(SEND, dest_pid, buffer, buffer_len);
}

/*
* sysputs
*
* @desc:	signals a ipc_recv for the current process
*
* @param:	from_pid	string message to display on console
*		buffer		buffer for receiving the ipc message
*		buffer_len	length of the receive buffer
*		
* @output:	rc		returns the number of bytes that have been received, in exceptional cases, the following will be returned
*				-1	invalid pid
*				-2	loopback pid
*				-3 	other ipc errors		
*/
int sysrecv(unsigned int *from_pid, void *buffer, int buffer_len)
{
	return syscall(RECV, from_pid, buffer, buffer_len);
}

/*
* syssighandler
*
* @desc:	signals a signal installation for a user proc
*
* @param:	signal		string message to display on console
*		new_handler	buffer for receiving the ipc message
*		old_handler	length of the receive buffer
*		
* @output:	rc		returns the status of the signal installation
*				0	successfully installed signal
*				-1	invalid signal
*				-2	invalid handler address 
*/
int syssighandler(int signal, void (*new_handler)(void *), void (** old_handler)(void *))
{
	return syscall(SIG_HANDLER, signal, new_handler, old_handler);
}

/*
* sigreturn
*
* @desc:	signals a change of stack pointer address for a user proc
*
* @param:	old_sp		user proc stack pointer address
*/
void sigreturn(void *old_sp)
{
	return syscall(SIG_RETURN, old_sp);
}

/*
* syskill
*
* @desc:	signals a signal delivery to the targetted user proc
*
* @param:	pid		target proc for signal delivery
*		sig_no		target signal for delivery
*		
* @output:	rc		returns the status of the signal delivery
*				0	successful signal delivery to user proc
*				-18	invalid pid
*/
int syskill(int pid, int sig_no)
{
	return syscall(SIG_KILL, pid, sig_no);
}

/*
* syssigwait
*
* @desc:	signals a user proc to be suspended until a signal is delivered to it
*
* @output:	rc		returns the unblocking delivered signal number
*/
int syssigwait(void)
{
	return syscall(SIG_WAIT);
}
