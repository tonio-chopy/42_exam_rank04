/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_popen.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaualik <marvin@d42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 19:47:42 by alaualik          #+#    #+#             */
/*   Updated: 2025/09/10 19:49:53 by alaualik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

int ft_popen(const char *file, char *const argv[], char type)
{
    if (!file || !argv || (type != 'r' && type != 'w'))
        return -1;
    int fd[2];
    pipe(fd);
    if (fork() == 0)
    {
        dup2(fd[type == 'r'], type == 'r');
        close(fd[1]);
        close(fd[0]);
        execvp(file, argv);
        exit(-1);
    }
    close(fd[type == 'r']);
    return (fd[type != 'r']);
}
