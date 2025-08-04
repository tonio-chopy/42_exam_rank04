/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsilva-x <lsilva-x@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 18:01:33 by lsilva-x          #+#    #+#             */
/*   Updated: 2025/07/18 20:07:33 by lsilva-x         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int	picoshell(char **cmds[]);

int	picoshell(char **cmds[])
{
	int		i;
	int		pid;
	int		exit_code;
	int		status;
	int		prev_fd;
	int		fd[2];

	i = 0;
	pid = 0;
	exit_code = 0;
	prev_fd = -1;
	status = 1;
	while (cmds[i])
	{
		// if exist a next command it CREATE an pipe
		if (cmds[i + 1] && pipe(fd))
			return (1);
		pid = fork();
		
		//handle error pipe
		if (pid == -1)
		{
			if (cmds[i + 1])
			{
				close(fd[0]);
				close(fd[1]);
			}
			return (1);
		}
		//child process
		if (pid == 0)
		{
			//if the prev_fd is != -1 the command must read from the previous fd
			if (prev_fd != -1)
			{
				if (dup2(prev_fd, STDIN_FILENO) == -1)
					exit (1);
				close (prev_fd);
			}
			//if exist next command it must redirect by write in the correct pipe
			if (cmds[i + 1])
			{
				close(fd[0]);
				if (dup2(fd[1], STDOUT_FILENO) == -1)
					exit (1);
				close (fd[1]);
			}
			execvp(cmds[i][0], cmds[i]);
			exit(1); // this line only will be execute if execvp failed
		}
		//parent process
		if (prev_fd != -1)
			close (prev_fd);
		if (cmds[i + 1])
		{
			close(fd[1]); // close the oldsest pipe write channel
			prev_fd = fd[0];
		}
		i++;
	}
	while (wait(&status) != -1)
	{
		//WIFEXITED verify if the subprocess terminate with exit or ended correctly
		//WEXITSTATUS return the exit status of the child process
		if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
			exit_code = 1; // it must be WEXITSTATUS(status) (or not)
	}
	return (exit_code);
}

/*
1 - verify if exist next command, if yes create a new pipe
2 - crate a fork and handle error
3 - (child process) verify if the previous fd is different from -1
	3.1 - if true redirect the stdint to the previous_fd and close it after
	3.2 - if not, verify if exist an next command
	3.3 - close the read fd (fd[0])
	3.4 - redirect the stdoutput from the current process to the fd[1] and close it
	3.5 - call excevp
4 - (parente process) verify if the prev_fd is != from -1, and close it 
	4.1 - verify if exist next command
		4.1.1 - if exist close the current write fd[1]
		4.1.2 - set the prev_fd to be fd[0];
*/


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