/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   v2.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsilva-x <lsilva-x@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 19:37:41 by lsilva-x          #+#    #+#             */
/*   Updated: 2025/07/30 19:55:56 by lsilva-x         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>

int sandbox(void (*f)(void), unsigned int timeout, bool verbose);

void	handler_function(int sig)
{
	(void)sig;
}

int sandbox(void (*f)(void), unsigned int timeout, bool verbose)
{
	struct sigaction sa;
	int				status;
	pid_t			pid;
	
	sa.sa_handler = handler_function;
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
	alarm(timeout);
	if(waitpid(pid, &status, 0) == -1)
	{
		if (errno == EINTR)
		{
			kill (pid, SIGKILL);
			waitpid(pid, NULL, 0);
			if (verbose)
				printf ("Bad function: timed out after %d seconds\n", timeout);
			return (0);
		}
	}
	if (WIFEXITED(status))
	{
		if (WEXITSTATUS(status) == 0)
		{
			if (verbose)
				printf("Nice function!\n");
			return (1);
		}
		else
		{
			if (verbose)
				printf("Bad function: exited with code %d\n", WEXITSTATUS(status));
			return(0);
		}
	}
	if (WIFSIGNALED(status))
	{
		int		sig;
		sig = WTERMSIG(status);
		if (verbose)
			printf("Bad function: %s\n", strsignal(sig));
		return (0);
	}
	return (-1);
}

void	compare(int res, int expected)
{
	printf ("EXPECTED: %d | RECIVE: %d\n", res, expected);
}

void	good_function(void)
{
	exit(0);
}

void	bad_function(void)
{
	exit(33);
}

void	time_out_function(void)
{
	while(1)
	{
		continue ;
	}
}


void	seg_function(void)
{
	char	*s = NULL;
	
	*s = 'a';
}

int main(void)
{
	compare(sandbox(good_function, 5, true), 1);
	compare(sandbox(bad_function, 5, true), 0);
	compare(sandbox(time_out_function, 2, true), 0);
	compare(sandbox(seg_function, 2, true), 0);
}