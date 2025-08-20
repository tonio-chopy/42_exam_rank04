/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tymo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaualik <marvin@d42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 11:26:55 by alaualik          #+#    #+#             */
/*   Updated: 2025/08/20 11:26:56 by alaualik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ctype.h>
#include <stdio.h>

void    unexpected(char c)
{
    if (c)
        printf("Unexpected token '%c'\n", c);
    else
        printf("Unexpected end of input\n");
}

int    accept(char **s, char c)
{
    if (**s == c)
    {
        (*s)++;
        return (1);
    }
    return (0);
}

int        parse_add(char **s);

int    parse_factor(char **s)
{
    int    result;

    if (accept(s, '('))
    {
        result = parse_add(s);
        if (result == -1)
            return (-1);
        if (!accept(s, ')'))
        {
            if (**s)
                unexpected(**s);
            else
                unexpected('\0');
            return (-1);
        }
    }
    else if (isdigit(**s))
    {
        result = **s - '0';
        (*s)++;
    }
    else
    {
        if (**s)
            unexpected(**s);
        else
            unexpected('\0');
        return (-1);
    }
    return (result);
}

int    parse_multi(char **s)
{
    int    left;
    int    right;

    left = parse_factor(s);
    if (left == -1)
        return (-1);
    while (accept(s, '*'))
    {
        right = parse_factor(s);
        if (right == -1)
            return (-1);
        left = left * right;
    }
    return (left);
}

int    parse_add(char **s)
{
    int    left;
    int    right;

    left = parse_multi(s);
    if (left == -1)
        return (-1);
    while (accept(s, '+'))
    {
        right = parse_multi(s);
        if (right == -1)
            return (-1);
        left = left + right;
    }
    return (left);
}

int    main(int ac, char **av)
{
    int        result;
    char    *s;

    if (ac != 2)
        return (1);
    s = av[1];
    result = parse_add(&s);
    if (result == -1 || *s != '\0')
    {
        if (result != -1)
            unexpected(*s);
        return (1);
    }
    printf("%d\n", result);
    return (0);
}
