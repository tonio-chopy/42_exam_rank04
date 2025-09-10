/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vbc.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alaualik <marvin@d42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 19:58:44 by alaualik          #+#    #+#             */
/*   Updated: 2025/09/10 20:11:01 by alaualik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vbc.h"

node *parse_nbr(char **s)
{
    if(!isdigit(**s))
    {
        unexpected(**s);
        return NULL;
    }
    node n = { .type = VAL, .val = **s - '0', .l = NULL, .r = NULL };
    (*s)++;
    return new_node(n);
}

node *parse_parenth(char **s)
{
    if (accept(s, '('))
    {
        node *expr = pars_expr(s);
        if(expect(s, ')'))
            return expr;
        else
        {
            destroy_tree(expr);
            return NULL;
        }
    }
    return parse_nbr(s);
}

node *parse_term(char **s)
{
    node *left = parse_parenth(s);
    while (accept(s, '*'))
    {
        node *right = parse_parenth(s);
        if (!right)
        {
            destroy_tree(left);
            return NULL;
        }
        node n = { .type = MULTI, .l = left, .r = right };
        left = new_node(n);
    }
    return (left);
}

node	*pars_expr(char **s)
{
    node *left = parse_term(s);
    while (accept(s, '+'))
    {
        node *right = parse_term(s);
        if(!right)
        {
            destroy_tree(left);
            return NULL;
        }
        node n = { .type = ADD, .l = left, .r = right };
        left = new_node(n);
    }
    return (left);
}
