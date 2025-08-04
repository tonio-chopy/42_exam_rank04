/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   v2.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsilva-x <lsilva-x@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 19:24:07 by lsilva-x          #+#    #+#             */
/*   Updated: 2025/07/30 19:35:56 by lsilva-x         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

int ft_popen(const char *file, char *const argv[], char type);

int ft_popen(const char *file, char *const argv[], char type)
{
	pid_t	pid;
	int		fd[2];
	
	if (!file || !argv || (type != 'r' && type != 'w'))
		return (-1);
	if (pipe(fd) == -1)
		return (-1);
	pid = fork();
	if (pid == -1)
	{
		close(fd[0]); //!close the fds
		close(fd[1]); //!close the fds
		return (-1);
	}
	if (pid == 0)
	{
		if (type == 'r')
		{
			close(fd[0]);
			if (dup2(fd[1], STDOUT_FILENO) == -1)
			{
				close (fd[1]);
				exit (-1);
			}
			close (fd[1]);
		}
		else
		{
			close(fd[1]);
			if (dup2(fd[0], STDIN_FILENO) == -1)
			{
				close(fd[0]);
				exit (-1);
			}
			close(fd[0]);
		}
		execvp(file, argv);
		exit (-1);
	}
	if (type == 'r')
	{
		close(fd[1]);
		return (fd[0]);
	}
	else
	{
		close(fd[0]);
		return (fd[1]);
	}
	return (-1);
}

#include <stdio.h>

int main()
{
    int  fd;
    char line[9999];
	int	b;

    fd = ft_popen("ls", (char *const []){"ls", NULL}, 'r');
    while (b = read(fd, line, 9999))
	{
		line[b] = '\0';
		printf("%s", line);
	}
	close(fd);
    return (0);
}