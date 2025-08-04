/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsilva-x <lsilva-x@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 21:15:12 by lsilva-x          #+#    #+#             */
/*   Updated: 2025/07/31 21:56:45 by lsilva-x         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   givenc_code.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsilva-x <lsilva-x@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 18:11:38 by lsilva-x          #+#    #+#             */
/*   Updated: 2025/07/29 18:11:48 by lsilva-x         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct node {
    enum {
        ADD,
        MULTI,
        VAL
    }   type;
    int val;
    struct node *l;
    struct node *r;
}   node;

node	*parse_multiplication(char **s);
node	*parse_addition(char **s);
node	*parse_primary(char **s);

node    *new_node(node n)
{
    node *ret = calloc(1, sizeof(n));
    if (!ret)
        return (NULL);
    *ret = n;
    return (ret);
}

void    destroy_tree(node *n)
{
    if (!n)
        return ;
    if (n->type != VAL)
    {
        destroy_tree(n->l);
        destroy_tree(n->r);
    }
    free(n);
}

void    unexpected(char c)
{
    if (c)
        printf("Unexpected token '%c'\n", c);
    else
        printf("Unexpected end of file\n");
}

int accept(char **s, char c)
{
    if (**s == c)
    {
        (*s)++;
        return (1);
    }
    return (0);
}

int expect(char **s, char c)
{
    if (accept(s, c))
        return (1);
    unexpected(**s);
    return (0);
}

node	*parse_primary(char **s)
{
	node	*ret;
	node	tmp;

	if (**s == '(')
	{
		(*s)++;
		ret = parse_addition(s);
		if (!ret || **s != ')')
		{
			destroy_tree();
			unexpected(**s);
			return (NULL);
		}
		(*s)++;
		return (ret);
	}
	if (isdigit(**s))
	{
		tmp.type = VAL;
		tmp.l = NULL;
		tmp.r = NULL;
		tmp.val = **s - '0';
		ret = new_node(tmp);
		(*s)++;
		return (ret);
	}
	unexpected(**s);
	return (NULL);
}


node	*parse_multiplication(char **s)
{
	node	*left;
	node	*right;
	node	tmp;

	left = parse_primary(s);
	if (!left)
		return (NULL);
	while (**s == '*')
	{
		(*s)++;
		right = parse_primary(s);
		if (!right)
		{
			destroy_tree(left);
			unexpected(**s);
			return (NULL);
		}
		tmp.type = MULTI;
		tmp.l = left;
		tmp.r = right;
		left = new_node(tmp);
		if (!left)
		{
			destroy_tree(right);
			return (NULL);
		}
		return (left);
	}
	return (left);
}


node	*parse_addition(char **s)
{
	node	*left;
	node	*right;
	node	tmp;

	left = parse_multiplication(s);
	while (**s == '+')
	{
		(*s)++;
		right = parse_multiplication(s);
		if (!right)
		{
			destroy_tree(left);
			return (NULL);
		}
		tmp.val = ADD;
		tmp.l = left;
		tmp.r = right;
		left = new_node(tmp);
		if (!left)
		{
			destroy_tree(right);
			return (NULL);
		}
		return (left);
	}
	return (left);
}

node    *parse_expr(char **s)
{
    node	*ret = parse_addition(s);

	if (!ret)
		return (NULL);
    if (**s) 
	{
		destroy_tree(ret);
		unexpected(**s);
		return (NULL);
	}
    return (ret);
}

int eval_tree(node *tree)
{
    switch (tree->type)
    {
        case ADD:
            return (eval_tree(tree->l) + eval_tree(tree->r));
        case MULTI:
            return (eval_tree(tree->l) * eval_tree(tree->r));
        case VAL:
            return (tree->val);
    }
	return (0);
}

int main(int argc, char **argv)
{
	char	*str;
	
	if (argc != 2)
		return (1);
	str = argv[1];
    node *tree = parse_expr(&str);
    if (!tree)
        return (1);
    printf("%d\n", eval_tree(tree));
    destroy_tree(tree);
}