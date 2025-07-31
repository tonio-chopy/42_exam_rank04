/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reference.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsilva-x <lsilva-x@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 17:34:41 by lsilva-x          #+#    #+#             */
/*   Updated: 2025/07/30 17:34:41 by lsilva-x         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int picoshell(char **cmds[])
{
    if (!cmds || !cmds[0])
        return 1;

    int fd[2];
    int in_fd = -1;
    pid_t pid;
    int status;
    int ret = 0;
    int i = 0;
    int children = 0;

    while (cmds[i])
    {
        if (cmds[i + 1])
        {
            if (pipe(fd) == -1)
                return 1;
        }
        else
        {
            fd[0] = -1;
            fd[1] = -1;
        }

        pid = fork();
        if (pid < 0)
            return 1;

        if (pid == 0)
        {
            if (in_fd != -1)
            {
                dup2(in_fd, 0);
                close(in_fd);
            }
            if (fd[1] != -1)
            {
                dup2(fd[1], 1);
                close(fd[1]);
                close(fd[0]);
            }
            execvp(cmds[i][0], cmds[i]);
            exit(1);
        }

        children++;

        if (in_fd != -1)
            close(in_fd);
        if (fd[1] != -1)
            close(fd[1]);

        in_fd = fd[0];
        i++;
    }

    if (in_fd != -1)
        close(in_fd);

    while (children--)
    {
        if (wait(&status) == -1)
            ret = 0;
        else if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
            ret = 0;
    }

    return ret;
}

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

/* Function prototype for picoshell */
int picoshell(char **cmds[]);

static int    count_cmds(int argc, char **argv)
{
    int    count = 1;
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "|") == 0)
            count++;
    }
    return (count);
}

int    main(int argc, char **argv)
{
    if (argc < 2)
        return (fprintf(stderr, "Usage: %s cmd1 [args] | cmd2 [args] ...\n", argv[0]), 1);

    int    cmd_count = count_cmds(argc, argv);
    char    ***cmds = calloc(cmd_count + 1, sizeof(char **));
    if (!cmds)
        return (perror("calloc"), 1);

    int    i = 1, j = 0;
    while (i < argc)
    {
        int    len = 0;
        while (i + len < argc && strcmp(argv[i + len], "|") != 0)
            len++;
        cmds[j] = calloc(len + 1, sizeof(char *));
        if (!cmds[j])
            return (perror("calloc"), 1);
        for (int k = 0; k < len; k++)
            cmds[j][k] = argv[i + k];
        cmds[j][len] = NULL;
        i += len + 1;
        j++;
    }
    cmds[cmd_count] = NULL;

    int    ret = picoshell(cmds);

    // Clean up
    for (int i = 0; cmds[i]; i++)
        free(cmds[i]);
    free(cmds);

    return (ret);
}