/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsilva-x <lsilva-x@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 20:30:14 by lsilva-x          #+#    #+#             */
/*   Updated: 2025/07/21 22:23:43 by lsilva-x         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h> //bool
#include <unistd.h> //fork, alarm, kill
#include <sys/wait.h> //waitpid
#include <stdio.h> //printf
#include <stdlib.h> //exit
#include <signal.h> //sigaction, strsignal, sigemptyset
#include <string.h> //strsignal
#include <sys/types.h>
#include <errno.h>

/*

-----------_FT_SANDBOX_----------------------------

Error function -> -1
Good fuction -> 0
Bad function -> 1 (terminate or stopped by a signal | exit with another code (!= 0) | time out)


pid_t waitpid (pid_t pid, int *status_ptr, int options);

pid > 0 -> waits for termination of the specific child
pid = 0 -> wait for termination of any child whose process groupd ID is equal
pid = -1 -> waits for termination of any child


status_ptr -> ptr do the variable tha will store the return status value

options
0 -> default behaviot (wait until the chidl terminates)

PROCESS (normally) -> exit _exit _Exit
WIFEXITED -> verify if a subprocess terminate
WEXITSTATUS -> verify the status of the subprocess already terminate

SIGNAL (abnormally) -> SIGKILL, SIGSEGV(signal segmentation fault), SIGTERM
WIFSIGNALED -> verfiy if a subprocess was terminate by a signal
WTERMSIG -> verify what was the signal that terminate that process

EINTR (Interrupted system call) is an error code used by certain system calls
read(), write(), wait(), when they are interrupted by a signal before they complete.
*/

static pid_t child_pid;

//Function that will be set in the sigaction
void	alarm_handler(int sig)
{
	(void)sig;
}

int sandbox(void (*f)(void), unsigned int timeout, bool verbose);

int sandbox(void (*f)(void), unsigned int timeout, bool verbose)
{
	struct sigaction	sa;
	pid_t				pid;
	int					status;

	sa.sa_handler = alarm_handler;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGALRM, &sa, NULL);

	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		f();
		exit(0);
	}
	child_pid = pid;
	alarm(timeout);
	if (waitpid(pid, &status, 0) == -1)
	{
		if (errno == EINTR)
		{
			kill(pid, SIGKILL);
			waitpid(pid, NULL , 0); // kill the process zombie
			if (verbose)
				printf ("Bad function: timed out after %d seconds\n", timeout);
			return (0);
		}
		return (-1);
	}

	if (WIFEXITED(status))
	{
		if (WEXITSTATUS(status) == 0)
		{
			if (verbose)
				printf ("Nice function!\n");
			return (1); //nice function
		}
		else
		{
			if (verbose)
				printf ("Bad function: exited with code %d\n", WEXITSTATUS(status));
			return (0); //bad function
		}
	}
	if (WIFSIGNALED(status))
	{
		int sig = WTERMSIG(status);
		if (verbose)
			printf ("Bad function: %s\n", strsignal(sig));
		return (0);
	}
	return (-1);
}

 void nice_function(void)
 {
    return;
 }


void bad_function_exit_code(void)
{
    // Esta función termina con código de error
    exit(1);
}

void bad_function_segfault(void)
{
    // Esta función causa segmentation fault
    int *ptr = NULL;
    *ptr = 42;
}

void bad_function_timeout(void)
{
    // Esta función nunca termina
    while (1) {}
}

void bad_function_abort(void)
{
    // Esta función llama abort()
    abort();
}

int main()
{
    int result;

    printf("Testing nice function:\n");
    result = sandbox(nice_function, 5, true);
    printf("Result: %d\n\n", result);  // Esperado: 1

    printf("Testing bad function (segfault):\n");
    result = sandbox(bad_function_segfault, 5, true);
    printf("Result: %d\n\n", result);  // Esperado: 0

    printf("Testing bad function (timeout):\n");
    result = sandbox(bad_function_timeout, 2, true);
    printf("Result: %d\n\n", result);  // Esperado: 0

    return 0;
}