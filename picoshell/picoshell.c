/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   picoshell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaualik <marvin@d42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 19:51:19 by alaualik          #+#    #+#             */
/*   Updated: 2025/09/10 19:52:52 by alaualik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

 void safe_close(int *fd)
{
    if (*fd != -1)
    {
        close(*fd);
        *fd = -1;
    }
}

int picoshell(char **cmds[])
{
    int fd[2] = {-1 , -1};
    int i = -1;
    int prev = -1;
    pid_t pid;

    while(cmds[++i])
    {
        if (cmds[i + 1])
            pipe(fd);
        if ((pid = fork()) < 0)
            return -1;
        if (!pid)
        {
            if (prev >= 0)
            {
                dup2(prev, 0);
                close(prev);
            }
            if (cmds[i + 1])
                dup2(fd[1], 1);
            safe_close(&fd[0]);
            safe_close(&fd[1]);
            execvp(cmds[i][0], cmds[i]);
            exit(1);
        }
        if (prev >= 0)
            close(prev);
        prev = cmds[i + 1] ? fd[0] : -1;
        safe_close(&fd[1]);
    }
    while(wait(NULL) > 0);
    safe_close(&prev);
    return (0);
}
