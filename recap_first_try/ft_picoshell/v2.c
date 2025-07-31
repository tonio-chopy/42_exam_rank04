/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   v2.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsilva-x <lsilva-x@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 18:01:13 by lsilva-x          #+#    #+#             */
/*   Updated: 2025/07/30 19:22:53 by lsilva-x         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int	picoshell(char **cmds[]);

static void	close_fd(int fd[2])
{
	close(fd[0]);
	close(fd[1]);
}

int	picoshell(char **cmds[])
{
	if (!cmds || !cmds[0])
		return (1);

	int		i = 0;
	int		prev_fd = -1;
	int		status = 1;
	int		exit_code = 0;
	int		children = 0;
	pid_t	pid;
	int		fd[2];

	while (cmds[i])
	{
		if(cmds[i + 1])
		{
			if (pipe(fd) == -1)
				return (1);
		}
		else
		{
			fd[0] = -1;
			fd[1] = -1;
		}
		pid = fork();
		if (pid == -1)
		{
			if (cmds[i + 1])
				close_fd(fd);
			return (1);
		}
		if (pid == 0)
		{
			if (prev_fd != -1)
			{
				if(dup2(prev_fd, STDIN_FILENO) == -1)
				{
					if (cmds[i + 1]) //!verification
						close_fd(fd);
					close(prev_fd);
					exit (1);
				}
				close(prev_fd);
			}
			if (fd[1] != -1)
			{
				if (dup2(fd[1], STDOUT_FILENO) == -1)
				{
					if (cmds[i + 1]) //!verification
						close_fd(fd);
					exit (1);
				}
				close(fd[0]);
				close(fd[1]);
			}
			execvp(cmds[i][0], cmds[i]);
			exit(1);
		}
		children++;
		if (prev_fd != -1)
			close(prev_fd);
		if (fd[1] != -1)
			close(fd[1]);
		prev_fd = fd[0];
		i++;
	}
	if(prev_fd != -1)
		close(prev_fd);
	while (children--)
	{
		if (wait(&status) == -1)
			exit_code = 1;
		else if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
			exit_code = 1;
		else
			exit_code = 0;
	}
	return (exit_code);
}


#include <stdio.h>
#include <string.h>

static int count_cmds(int argc, char **argv)
{
    int count = 1;
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "|") == 0)
            count++;
    }
    return count;
}

int main(int argc, char **argv)
{
    if (argc < 2)
        return (fprintf(stderr, "Usage: %s cmd1 [args] | cmd2 [args] ...\n", argv[0]), 1);

    int cmd_count = count_cmds(argc, argv);
    char ***cmds = calloc(cmd_count + 1, sizeof(char **));
    if (!cmds)
        return (perror("calloc"), 1);

    // Parsear argumentos y construir array de comandos
    int i = 1, j = 0;
    while (i < argc)
    {
        int len = 0;
        while (i + len < argc && strcmp(argv[i + len], "|") != 0)
            len++;
        
        cmds[j] = calloc(len + 1, sizeof(char *));
        if (!cmds[j])
            return (perror("calloc"), 1);
        
        for (int k = 0; k < len; k++)
            cmds[j][k] = argv[i + k];
        cmds[j][len] = NULL;
        
        i += len + 1;  // Saltar el "|"
        j++;
    }
    cmds[cmd_count] = NULL;

    int ret = picoshell(cmds);

    // Limpiar memoria
    for (int i = 0; cmds[i]; i++)
        free(cmds[i]);
    free(cmds);

    return ret;
}
