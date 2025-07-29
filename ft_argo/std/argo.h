/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argo.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsilva-x <lsilva-x@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 18:37:42 by lsilva-x          #+#    #+#             */
/*   Updated: 2025/07/28 15:49:11 by lsilva-x         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ARGO_H
# define ARGO_H

#include <stdio.h> //printf, getc, ungetc
#include <stdlib.h> //malloc, calloc, realloc, free
#include <ctype.h> //isdigit
#include <unistd.h> //write
#include <stdbool.h>
#include <stdlib.h>

/*
------------------FT_ARGO-------------

`int getc(FILE *stream);` -> used to read a single char from a spefied input stream
`int ungetc(int c, FILE *stream)` -> pushes a char back onto an input stream
`int fscanf(FILE *stream, const char *format, ...)` -> used to read formatted input
from a file stream
*/

typedef struct	json {
	enum {
		MAP,
		INTEGER,
		STRING
	} type;
	union {
		struct {
			struct pair	*data;
			size_t		size;
		} map;
		int	integer;
		char	*string;
	};
}	json;

typedef struct	pair {
	char	*key;
	json	value;
}	pair;

void	free_json(json j);
int		argo(json *dst, FILE *stream);

#endif