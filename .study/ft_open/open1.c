/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsilva-x <lsilva-x@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 16:39:29 by lsilva-x          #+#    #+#             */
/*   Updated: 2025/07/24 16:59:09 by lsilva-x         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h> //pipe, fork, dup2, excvep, close
#include <stdlib.h> // exit

int ft_popen(const char *file, char *const argv[], char type);

static void	close_fds(int fds[2])
{
	close(fds[0]);
	close(fds[1]);
}

int ft_popen(const char *file, char *const argv[], char type)
{
	int		fds[2];
	int		pid;

	if (!file || !argv || (type != 'r' && type != 'w'))
		return (-1);
	if (pipe(fds) == -1)
		return (-1);
	pid = fork();
	if (pid == -1)
	{
		close_fds(fds);
		return (-1);
	}
	if (pid == 0)
	{
		if (type == 'r')
		{
			close(fds[0]);
			if (dup2(fds[1], STDOUT_FILENO) == -1)
			{
				close(fds[1]);
				exit (1);
			}
			close(fds[1]);
		}
		else
		{
			close(fds[1]);
			if (dup2(fds[0], STDIN_FILENO) == -1)
			{
				close(fds[0]);
				exit (1);
			}
			close(fds[0]);
		}
		execvp(file, argv);
		exit(1);
	}
	if (type == 'r')
	{
		close(fds[1]);
		return (fds[0]);
	}
	else
	{
		close(fds[0]);
		return (fds[1]);
	}
	return (-1);
}


#include <stdio.h>
#include <wait.h>

int main(int argc, char const *argv[])
{
 int fd;
 
 fd = ft_popen("wc", (char *const []){"wc", "-l", NULL}, 'w');
 if (fd == -1)
     return 1;
 write(fd, "line 1\n", 7);
 write(fd, "line 2\n", 7);
 write(fd, "line 3\n", 7);
 close(fd);  // wc mostrará "3"
//  wait(NULL);
	return 0;
}
