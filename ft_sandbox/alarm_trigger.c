/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alarm_trigger.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsilva-x <lsilva-x@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 20:39:00 by lsilva-x          #+#    #+#             */
/*   Updated: 2025/07/21 20:44:22 by lsilva-x         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <stdio.h>
#include <unistd.h> //alarm

void	handle_sig(int sig)
{
	printf ("Hello world\n");
	return ;
}

#define TIMER 5
int main(void)
{
	signal(SIGALRM, handle_sig);
	alarm(TIMER);
	printf ("wait\n");
	pause();
	printf("End\n");
	return 0;
}
