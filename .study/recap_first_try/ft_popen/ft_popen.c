/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_popen.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsilva-x <lsilva-x@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 15:53:54 by lsilva-x          #+#    #+#             */
/*   Updated: 2025/07/29 16:10:42 by lsilva-x         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>

int ft_popen(const char *file, char *const argv[], char type);


static void	close_fd(int fd[2])
{
	close(fd[0]);
	close(fd[1]);
}

int ft_popen(const char *file, char *const argv[], char type)
{
	int		fd[2];
	int		pid;
	
	if (!file || !argv || (type != 'r' && type != 'w'))
		return (-1);
	if (pipe(fd) == -1)
		return (-1);
	pid = fork();
	if (pid == -1)
		return (close_fd(fd), -1);
	if (pid == 0)
	{
		if (type == 'r')
		{
			close(fd[0]);
			if (dup2(fd[1], STDOUT_FILENO) == -1)
			{
				close(fd[1]);
				exit (-1);
			}
			close(fd[1]);
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
	int		b_read;

    fd = ft_popen("ls", (char *const []){"ls", NULL}, 'r');
	dup2(fd, STDIN_FILENO);
	close(fd);
	fd = ft_popen("wc", (char *const []){"wc", "-l", NULL}, 'w');
    while (b_read = read(0, line, 9999))
	{
		line[b_read] = '\0';
		write(fd, line, b_read);
	}
	close(fd);
    return (0);
}