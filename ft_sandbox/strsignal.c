/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strsignal.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsilva-x <lsilva-x@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 20:47:33 by lsilva-x          #+#    #+#             */
/*   Updated: 2025/07/21 20:50:43 by lsilva-x         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[])
{
	int		SIG = SIGINT;

	printf("Signal message: %s\n", strsignal(SIG));
	return 0;
}
