/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsilva-x <lsilva-x@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 16:31:30 by lsilva-x          #+#    #+#             */
/*   Updated: 2025/07/21 17:28:50 by lsilva-x         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h> // pipe, fork. dup2, execvp, close
#include <stdlib.h> // exit

int ft_popen(const char *file, char *const argv[], char type);

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
	{
		close(fd[0]);
		close(fd[1]);
		return (-1);
	}
	if (pid == 0)
	{
		if (type == 'r')
		{
			if(dup2(fd[1], STDOUT_FILENO) == -1)
			{
				close(fd[0]);
				close(fd[1]);
				return (-1);
			}
		}
		else
		{
			if(dup2(fd[0], STDIN_FILENO) == -1)
			{
				close(fd[0]);
				close(fd[1]);
				return (-1);
			}
		}
		close(fd[0]);
		close(fd[1]);
		execvp(file, argv);
		exit(-1);
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
}

# define BUFFER_SIZE 100
#include <stdio.h>

int	main() {
	int	fd = ft_popen("ls", (char *const []){"ls", NULL}, 'r');
	dup2(fd, 0);
	fd = ft_popen("grep", (char *const []){"grep", "c", NULL}, 'r');
	char	line[BUFFER_SIZE];
	int		b_read;
	while ((b_read = read(fd, line, BUFFER_SIZE)) != 0)
	{
		line[b_read] = '\0';
		printf("%s", line);
	}
	close(fd);
}