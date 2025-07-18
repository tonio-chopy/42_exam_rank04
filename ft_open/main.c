/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsilva-x <lsilva-x@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 15:47:51 by lsilva-x          #+#    #+#             */
/*   Updated: 2025/07/18 16:51:53 by lsilva-x         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

int ft_popen(const char *file, char *const argv[], char type);

/*
type
r - return fd from output
w - rturn fd from input

error = -1


fd[0] = read
fd[1] = write

type == 'r'
significa que o comando vai ser executado dentro de um processo filho
e tudo que ele escrever vai ser escrito no STDOUT do fd (1), pois o 
fd[]


r -> escreve no fd[1], redireciona o STDOUT para o fd[1]
w -> le do fd[0], redireciona o STDINT para o fd[0] e colocar a imagem
do processo por algum binário do sistema
*/

int ft_popen(const char *file, char *const argv[], char type)
{
	int		fd[2];
	pid_t	pid;

	if (!file || !argv || (type != 'r' && type!= 'w'))
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
			close(fd[0]);
			if (dup2(fd[1], STDOUT_FILENO) == -1)
				exit(1);
			close(fd[1]);
		}
		else
		{
			close(fd[1]);
			if (dup2(fd[0], STDIN_FILENO) == -1)
				exit(1);
			close(fd[0]);
		}
		execvp(file, argv);
		exit (1);
	}
	if (type == 'r')
		return (close(fd[1]), fd[0]);
	else
		return (close(fd[0]), fd[1]);
	return (0);
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
