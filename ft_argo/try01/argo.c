/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsilva-x <lsilva-x@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 18:37:30 by lsilva-x          #+#    #+#             */
/*   Updated: 2025/07/28 20:16:06 by lsilva-x         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "argo.h"

/*

------------FT_ARGO----------

int	argo(json *dst, FILE *stream);

dst -> ptr to where the AST will be build
stream -> the source to the AST

Handle only
-numbers
-strings
-maps

Handle escaping in string only for basckslashes and quotation marks

-------------RETURN-----------
1 -> okay
-1 -> failure

------------ERROR HANDLER-------
unexpected token -> print  "Unexpected token '%c'\n"
tokne is EOF -> "Unexpected end of input\n"

------------ UNION ------------
A union is a special type of struct used to allocate shared memory space for its members.
Instead of allocating memory for each member separately, all members share the same memory location.

It is useful when you need to store one of several types of data, but never more than one at a time.

The size of the union is equal to the size of its largest member.

*/

#define BUFFER_SIZE 99999

int	parse_string(json *dst, FILE *stream);
int	parse_int(json *dst, FILE *stream);
int	parse_map(json *dst, FILE *stream);
int parser(json *dst, FILE *stream);
static char	*ft_strdup(char *str);

int parser(json *dst, FILE *stream)
{
	int	c;

	c = peek(stream);
	if (c == '"')
		return (parse_string(dst, stream));
	else if (isdigit(c) || c == '-')
		return (parse_int(dst, stream));
	else if (c == '{')
		return (parse_map(dst, stream));
	else
	{
		unexpected(stream);
		return (-1);
	}
}

static char	*ft_strdup(char *str)
{
	size_t	str_s;
	int		i;
	char	*str_cpy;
	
	i = -1;
	str_s = -1;
	while (str[++str_s])
		continue ;
	str_cpy = (char *)malloc(sizeof(char) * (str_s + 1));
	while (str[++i])
		str_cpy[i] = str[i];
	str_cpy[i] = '\0';
	return (str_cpy);
}

int	parse_map(json *dst, FILE *stream)
{
	pair	*items; // array t de pares
	size_t	size;
	json	key;

	if (!expect(stream, '{'))
		return (-1);
	items = NULL;
	size = 0;
	while (!accept(stream, '}'))
	{
		items = realloc(items, sizeof(pair) * (size + 1));
		if (parse_string(&key, stream) == -1)
		{
						unexpected(stream);

			return (-1);
		}
		if (!expect(stream, ':'))
		{
			free(key.string);
			free(items);
			return (-1);
		}
		if (parser(&items[size].value, stream) == -1)
		{
			free(key.string);
			free(items);
			return (-1);
		}
		items[size].key = key.string;
		size++;
		if (!accept(stream, ',') && peek(stream) != '}')
		{
			unexpected(stream);
			free(items);
			return (-1);
		}
		dst->type = MAP;
		dst->map.size = size;
		dst->map.data = items;
		return (1);
	}
}

int	parse_int(json *dst, FILE *stream)
{
	int		n;

	if (fscanf(stream, "%d", &n) == 1)
	{
		dst->type = INTEGER;
		dst->integer = n;
		return (1);
	}
	unexpected(stream);
	return (-1);
}

int	parse_string(json *dst, FILE *stream)
{
	char	buffer[BUFFER_SIZE];
	char	c;
	int		i;

	if (!expect(stream, '"'))
		return (-1);
	i = 0;
	while(1)
	{
		c = getc(stream);
		if (c == EOF)
		{
			unexpected(stream);
			return (-1);
		}
		if (c == '"')
			break ;
		if (c == '\\')
		{
			c = getc(stream);
			if (c == EOF)
			{
				unexpected(stream);
				return (-1);
			}
		}
		buffer[i++] = c;
	}
	buffer[i] = '\0';
	dst->type = STRING;
	dst->string = ft_strdup(buffer); // ! verify this before
	return (1);
}

int	argo(json *dst, FILE *stream)
{
	return (parser(dst, stream));
}
