/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsilva-x <lsilva-x@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 13:58:40 by lsilva-x          #+#    #+#             */
/*   Updated: 2025/07/28 14:19:48 by lsilva-x         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>  //dup,fork,pipe,execvp, close
#include <stdlib.h> //exit

int ft_popen(const char *file, char *const argv[], char type);

static void close_fd(int fd[2])
{
	close(fd[0]);
	close(fd[1]);
}

int ft_popen(const char *file, char *const argv[], char type)
{
	int		pid;
	int		fd[2];

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
				exit(-1);
			}
			close(fd[0]);
		}
		execvp(file, argv);
		exit(-1);
	}
	if (type == 'r')
		return (close(fd[1]), fd[0]);
	else
		return (close(fd[0]), fd[1]);
}

#include <stdio.h>

int main()
{
	int	fd = ft_popen("wc", (char *const []){"wc", "-l", NULL}, 'w');
	dup2(fd, 1);
	close(fd);
	fd = ft_popen("ls", (char *const []){"ls", NULL}, 'r');
	char	line[999];
	int		b_read;
	while (b_read = read(fd, line, 999))
	{
		line[b_read] = '\0';
		// printf("%s", line);
		write(1, line, b_read);
	}
	close(fd);
	return (0);
}