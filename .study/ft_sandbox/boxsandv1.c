/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   boxsandv1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsilva-x <lsilva-x@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 17:30:15 by lsilva-x          #+#    #+#             */
/*   Updated: 2025/07/24 18:32:27 by lsilva-x         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

int sandbox(void (*f)(void), unsigned int timeout, bool verbose);

void	sig_handler(int sig)
{
	(void)sig;
}

int sandbox(void (*f)(void), unsigned int timeout, bool verbose)
{
	int		pid;
	int		status;
	struct sigaction sa;

	sa.sa_handler = sig_handler;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGALRM, &sa, 0);

	if (!f)
		return (0);
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
			kill(pid, SIGKILL);
			waitpid(pid, NULL, 0);
			if (verbose)
				printf("Bad function: timed out after %d seconds\n", timeout);
			return (0);
		}
		return (-1);
	}
	if (WIFEXITED(status))
	{
		if (WEXITSTATUS(status) == 0)
		{
			if (verbose)
				printf("Nice function!\n");
			return (0);
		}
		else
		{
			if (verbose)
				printf ("Bad function: exited with code %d\n", WEXITSTATUS(status));
			return (1);
		}
	}
	if (WIFSIGNALED(status))
	{
		int		sig;

		sig = WTERMSIG(status);
		if (verbose)
			printf("Bad function: %s\n", strsignal(sig));
		return (1);
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

// Helper to check sandbox result and print outcome
void check_result(const char *test_name, int expected, int actual) {
	if (expected == actual)
		printf("[PASS] %s (expected: %d, got: %d)\n", test_name, expected, actual);
	else
		printf("[FAIL] %s (expected: %d, got: %d)\n", test_name, expected, actual);
}

// Additional test functions
void bad_function_divzero(void) {
	int x = 1 / 0;
	(void)x;
}

void bad_function_sigfpe(void) {
	raise(SIGFPE);
}

void bad_function_sigterm(void) {
	raise(SIGTERM);
}

void bad_function_sigkill(void) {
	raise(SIGKILL);
}

void nice_function_sleep(void) {
	sleep(1);
}

int main(void)
{
	int result;

	// Test: nice function
	result = sandbox(nice_function, 5, false);
	check_result("nice_function", 0, result);

	// Test: nice function with sleep
	result = sandbox(nice_function_sleep, 5, false);
	check_result("nice_function_sleep", 0, result);

	// Test: bad function exit code
	result = sandbox(bad_function_exit_code, 5, false);
	check_result("bad_function_exit_code", 1, result);

	// Test: bad function segfault
	result = sandbox(bad_function_segfault, 5, false);
	check_result("bad_function_segfault", 1, result);

	// Test: bad function timeout
	result = sandbox(bad_function_timeout, 2, false);
	check_result("bad_function_timeout", 0, result);

	// Test: bad function abort
	result = sandbox(bad_function_abort, 5, false);
	check_result("bad_function_abort", 1, result);

	// Test: bad function division by zero
	result = sandbox(bad_function_divzero, 5, false);
	check_result("bad_function_divzero", 1, result);

	// Test: bad function SIGFPE
	result = sandbox(bad_function_sigfpe, 5, false);
	check_result("bad_function_sigfpe", 1, result);

	// Test: bad function SIGTERM
	result = sandbox(bad_function_sigterm, 5, false);
	check_result("bad_function_sigterm", 1, result);

	// Test: bad function SIGKILL
	result = sandbox(bad_function_sigkill, 5, false);
	check_result("bad_function_sigkill", 1, result);

	// Test: zero timeout (should always timeout)
	result = sandbox(nice_function_sleep, 0, false);
	check_result("nice_function_sleep_zero_timeout", 0, result);

	// Test: NULL function pointer (should not crash, but sandbox will fork and exit)
	result = sandbox(NULL, 5, false);
	check_result("null_function_pointer", 0, result);

	return 0;
}