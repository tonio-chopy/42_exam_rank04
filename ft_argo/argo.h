/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argo.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsilva-x <lsilva-x@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 18:37:42 by lsilva-x          #+#    #+#             */
/*   Updated: 2025/07/24 18:46:28 by lsilva-x         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ARGO_H
# define ARGO_H

#include <stdio.h> //printf, getc, ungetc
#include <stdlib.h> //malloc, calloc, realloc, free
#include <ctype.h> //isdigit
#include <unistd.h> //write
#include <stdbool.h>

/*
------------------FT_ARGO-------------

`int getc(FILE *stream);` -> used to read a single char from a spefied input stream
`int ungetc(int c, FILE *stream)` -> pushes a char back onto an input stream
`int fscanf(FILE *stream, const char *format, ...)` -> used to read formatted input
from a file stream
*/

int	argo(json *dst, FILE *stream);

#endif