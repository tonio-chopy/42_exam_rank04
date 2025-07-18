/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   picoshell_v2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsilva-x <lsilva-x@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 19:33:54 by lsilva-x          #+#    #+#             */
/*   Updated: 2025/07/18 20:20:15 by lsilva-x         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int		picoshell(char **cmds[]);

int picoshell(char **cmds[])
{
	int		i = 0;
	int		pid = 0;
	int		prev_fd = -1;
	int		code_status = 0;
	int		status = 0;
	int		fd[2];

	while (cmds[i])
	{
		if (cmds[i + 1] && pipe(fd))
			return (1);
		pid = fork();
		if (pid == -1)
		{
			if (cmds[i + 1])
			{
				close(fd[0]);
				close(fd[1]);
			}
			return (1);
		}
		if (pid == 0)
		{
			if (prev_fd != -1)
			{
				if(dup2(prev_fd, STDIN_FILENO) == -1)
					exit(-1);
				close (prev_fd);
			}
			if (cmds[i + 1])
			{
				close(fd[0]);
				if (dup2(fd[1], STDOUT_FILENO) == -1)
					exit(-1);
				close(fd[1]);
			}
			execvp(cmds[i][0], cmds[i]);
			exit (1); //this line will be exectued if execvp failed
		}
		//close the prev_fd if is already reset
		if (prev_fd == -1)
			close (prev_fd);
		if (cmds[i + 1])
		{
			close(fd[1]);
			prev_fd = fd[0];
		}
		i++;
	}
	while (wait(&status) != -1)
	{
		if (WIFEXITED(status) && WEXITSTATUS(status))
			code_status = WEXITSTATUS(status);
	}
	return (code_status);
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

/*
Main test steps

1. Count how many cmds ("|" is not a value to count);
2. char **cmds malloc the commands (an extra space to the NULL terminator)
3. iterate through the argv
	3.1 count how many token belongs to the same cmds
*/