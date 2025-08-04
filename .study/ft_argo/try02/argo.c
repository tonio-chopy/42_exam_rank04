#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>


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

int		expect(FILE *stream, char c);
void	unexpected(FILE *stream);
int		accept(FILE *stream, char c);
int		peek(FILE *stream);


int		parser(json *dst, FILE *stream);
char	*ft_strdup(char *str);
int		argo(json *dst, FILE *stream);
int		parser_string(json *dst, FILE *stream);
int		parser_integer(json *dst, FILE *stream);
int		parser_map(json *dst, FILE *stream);

char	*ft_strdup(char *str)
{
	int		i;
	char	*new_str;
	
	i = -1;
	while (str[++i])
		continue;
	new_str = (char *)malloc(sizeof(char) * (i + 1));
	i = -1;
	while (str[++i])
		new_str[i] = str[i];
	new_str[i] = '\0';
	return (new_str);
}

int		parser_string(json *dst, FILE *stream)
{
	char	buffer[99999];
	char 	c;
	int		i;
	
	if (!expect(stream, '"'))
		return (-1);
	i = 0;
	while (1)
	{
		c = getc(stream);
		if (c == EOF)
		{
			unexpected(stream);
			return (-1);
		}
		if (c == '"')
			break;
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
	dst->string = ft_strdup(buffer);
	return (1);
}

int	parser_integer(json *dst, FILE *stream)
{
	int		c;

	if (fscanf(stream, "%d", &c) == 1)
	{
		dst->type = INTEGER;
		dst->integer = c;
		return (1);
	}
	unexpected(stream);
	return (-1);
}

int		parser_map(json *dst, FILE *stream)
{
	pair	*items;
	size_t	size;
	json	key;

	if (!expect(stream, '{'))
		return (-1);
	items = NULL;
	size = 0;
	while (!accept(stream, '}'))
	{
		items = realloc(items, sizeof(pair) * (size + 1));
		if(parser_string(&key, stream) == -1) //string
		{
			unexpected(stream);
			return (-1);
		}
		if(!expect(stream, ':')) //expected
		{
			free(key.string);
			free(items);
			return (-1);
		}
		if(parser(&items[size].value, stream) == -1) //parser
		{
			free(key.string);
			free(items);
			return (-1);
		}
		items[size].key = key.string;
		size++;
		if(!accept(stream, ',') && peek(stream) != '}') // invalid format
		{
			unexpected(stream);
			free(items);
			return (-1);
		}
	}
	dst->type = MAP;
	dst->map.size = size;
	dst->map.data = items;
	return (1);
}

int		parser(json *dst, FILE *stream)
{
	char	c;

	c = peek(stream);
	if (c == '"')
		return (parser_string(dst, stream));
	else if (isdigit(c) || c == '-')
		return (parser_integer(dst, stream));
	else if (c == '{')
		return (parser_map(dst, stream));
	else
	{
		unexpected(stream);
		return (-1);
	}
}

int		argo(json *dst, FILE *stream)
{
	return (parser(dst, stream));
}

int	peek(FILE *stream)
{
	int	c = getc(stream);
	ungetc(c, stream);
	return c;
}

void	unexpected(FILE *stream)
{
	if (peek(stream) != EOF)
		printf("unexpected token '%c'\n", peek(stream));
	else
		printf("unexpected end of input\n");
}

int	accept(FILE *stream, char c)
{
	if (peek(stream) == c)
	{
		(void)getc(stream);
		return 1;
	}
	return 0;
}

int	expect(FILE *stream, char c)
{
	if (accept(stream, c))
		return 1;
	unexpected(stream);
	return 0;
}

void	free_json(json j)
{
	switch (j.type)
	{
		case MAP:
			for (size_t i = 0; i < j.map.size; i++)
			{
				free(j.map.data[i].key);
				free_json(j.map.data[i].value);
			}
			free(j.map.data);
			break ;
		case STRING:
			free(j.string);
			break ;
		default:
			break ;
	}
}

void	serialize(json j)
{
	switch (j.type)
	{
		case INTEGER:
			printf("%d", j.integer);
			break ;
		case STRING:
			putchar('"');
			for (int i = 0; j.string[i]; i++)
			{
				if (j.string[i] == '\\' || j.string[i] == '"')
					putchar('\\');
				putchar(j.string[i]);
			}
			putchar('"');
			break ;
		case MAP:
			putchar('{');
			for (size_t i = 0; i < j.map.size; i++)
			{
				if (i != 0)
					putchar(',');
				serialize((json){.type = STRING, .string = j.map.data[i].key});
				putchar(':');
				serialize(j.map.data[i].value);
			}
			putchar('}');
			break ;
	}
}

int	main(int argc, char **argv)
{
	if (argc != 2)
		return 1;
	char *filename = argv[1];
	FILE *stream = fopen(filename, "r");
	json	file;
	if (argo (&file, stream) != 1)
	{
		free_json(file);
		return 1;
	}
	serialize(file);
	printf("\n");
}