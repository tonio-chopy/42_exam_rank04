/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   picoshell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsilva-x <lsilva-x@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 16:11:47 by lsilva-x          #+#    #+#             */
/*   Updated: 2025/07/30 17:50:30 by lsilva-x         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>


int	picoshell(char **cmds[]);

static void	close_fd(int fd[2])
{
	close(fd[0]);
	close(fd[1]);
}

int	picoshell(char **cmds[])
{
	if (!cmds || !cmds[0]) //!basic validation
		return (1);

	int		i = 0;
	pid_t	pid; //!always use pid instead int
	int		prev_fd = -1;
	int		status = 1;
	int		exit_code = 0;
	int		fd[2];
	int		children = 0; //? stil don't know the propouse of this variable


	while (cmds[i])
	{
		if (cmds[i + 1])
		{
			if (pipe(fd) == -1)
				return (1);
		}
		else //!set an else to restart the fd values
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
				if (dup2(prev_fd, STDIN_FILENO) == -1)
				{
					if (cmds[i + 1])
						close_fd(fd);
					exit (1);
				}
				close(prev_fd);
			}
			if (fd[1] != -1) //! change the verification
			{
				if (dup2(fd[1], STDOUT_FILENO) == -1)
				{
					if (cmds[i + 1])
						close_fd(fd);
					exit (1);
				}
				close(fd[1]); //! close both i the end of the second verification
				close(fd[0]);
			}
			execvp(cmds[i][0], cmds[i]);
			exit(1);
		}
		children++; //!add children variable
		if (prev_fd != -1)
			close(prev_fd);
		if (fd[1] != -1) //! change the verification
			close(fd[1]);
		prev_fd = fd[0]; //! always set this
		i++;
	}
	if (prev_fd != -1) //! add this verification
		close (prev_fd);
	while (children--) //! complete refactory from this block
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
