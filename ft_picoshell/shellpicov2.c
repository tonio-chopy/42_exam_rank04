/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shellpicov2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsilva-x <lsilva-x@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 17:03:17 by lsilva-x          #+#    #+#             */
/*   Updated: 2025/07/24 17:29:09 by lsilva-x         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h> //close, fork, excevp, dup2, pipe
#include <sys/wait.h> // wait
#include <stdlib.h> // exit

int	picoshell(char **cmds[]);

static void		close_fds(int fds[2])
{
	close(fds[0]);
	close(fds[1]);
}

int	picoshell(char **cmds[])
{
	int		i = 0;
	int		prev_fd = -1;
	int		pid;
	int		fds[2];
	int		status = 1;
	int		exit_code = 0;
	
	while (cmds[i])
	{
		if (cmds[i + 1])
		{
			if (pipe(fds) == -1)
				return (1);
		}
		pid = fork();
		if (pid == -1)
		{
			if (cmds[i + 1])
				close_fds(fds);
			return (-1);
		}
		if (pid == 0)
		{
			if (prev_fd != -1)
			{
				if (dup2(prev_fd, STDIN_FILENO) == -1)
				{
					if (cmds[i + 1])
						close_fds(fds);
					exit (1);
				}
				close (prev_fd); //prev close
			}
			if (cmds[i + 1])
				close(fds[0]); //read close
			if (cmds[i + 1])
			{
				if (dup2(fds[1], STDOUT_FILENO) == -1)
				{
					if (cmds[i + 1])
						close(fds[1]);
					exit(1);
				}
				close(fds[1]); // write close
			}
			execvp(cmds[i][0], cmds[i]);
			exit(1);
		}
		if (prev_fd != -1)
			close (prev_fd);
		if (cmds[i + 1])
		{
			prev_fd = fds[0];
			close(fds[1]);
		}
		i++;
	}
	while (wait(&status) != -1)
	{
		if (WEXITSTATUS(status) != 0)
			exit_code = 1;
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