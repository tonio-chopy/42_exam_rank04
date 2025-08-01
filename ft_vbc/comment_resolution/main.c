#include <unistd.h>
#include <stdio.h>
#include <malloc.h>
#include <ctype.h>



typedef struct node {
    enum {
        ADD,
        MULTI,
        VAL
    }   type; //! type is a enum type that can only accept the previous enum set values
    int val; //! the current node value
    struct node *l; //! Left node
    struct node *r; //! Right node
}   node;

//? Create and return a new node with the same values of (node n) pass as parameter
node	*new_node(node n)
{
    node *ret = calloc(1, sizeof(n));
    if (!ret)
        return (NULL);
    *ret = n;
    return (ret);
}

node 	*parse_multiplication(char **str);
node	*parse_addtion(char **str);
node 	*parse_primary(char **str);

//? Recursively destroy the left and right nodes
void    destroy_tree(node *n)
{
    if (!n)
        return ;
	//!this condition referest to the fact of VAl are leaf nodes and they don't had childrens
    if (n->type != VAL) 
    {
        destroy_tree(n->l); //? Recursively destroy the left nodes
        destroy_tree(n->r); //? Recursively destroy the left nodes
    }
    free(n);
}

//?Function to correctly handle the error input cases
void    unexpected(char c)
{
    if (c)
        printf("Unexpected token '%c'\n", c);
    else
        printf("Unexpected end of file\n");
}

/*
? Verify if the currenct value of the string is equal to what we expected
? if is the string is set to be the next value (jump the current) and return 1
? if not the function return 0;
*/
int accept(char **s, char c)
{
    if (**s == c)
    {
        (*s)++;
        return (1);
    }
    return (0);
}
//!Almost the same of accept function but it pop an error if the letter is not what expect
int expect(char **s, char c)
{
    if (accept(s, c))
        return (1);
    unexpected(**s);
    return (0);
}

node *parse_primary(char **str)
{
	node	*res;
	node	tmp;

	if (**str == '(')
	{
		(*str)++; //? jump the (
		res = parse_addtion(str); //? enter to the same recursion to verify if is + if not verify until found a *
		if(!res || **str != ')')
		{
			destroy_tree(res);
			unexpected(**str); //! unexpected token
			return (NULL);
		}
		(*str)++; //? jump the ) (end of parenthesis)
		return (res);
	}

	//!if is not a open of parenthesis the code fall in the normal case leaf the number it self
	if (isdigit(**str))
	{
		tmp.type = VAL;
		tmp.l = NULL;
		tmp.r = NULL;
		tmp.val = **str - '0';

		res = new_node(tmp);
		(*str)++;
		return (res);
	}
	unexpected(**str); //!unexpected token
	return (NULL);
}

node *parse_multiplication(char **str)
{
	node	*left;
	node	*right;
	node	tmp;

	left = parse_primary(str);
	if (!left)
		return (NULL);
	while (**str == '*')
	{
		(*str)++;
		//!the same as addition but in this case parse_primary is called;
		right = parse_primary(str);
		if (!right)
		{
			destroy_tree(left);
			return (NULL);
		}
		//? create the node
		tmp.type = MULTI;
		tmp.l = left;
		tmp.r = right;
		tmp.val = 0; //? 0 because this node represent the * operator

		left = new_node(tmp);
		if (!left)
		{
			destroy_tree(right);
			return (NULL);
		}
	}
	return (left);
}

node	*parse_addtion(char **str)
{
	node	*left;
	node	*right;
	node	tmp;

	//!the AST is build based on order of mathematical precedence and Left-associative
	//!that means we'll try find a multiplicatio node in the string to build the most important node
	left = parse_multiplication(str);
	while (**str == '+')
	{
		(*str)++; //! consume the '+' operator

		//!This function is called because of the order of mathematical precedence
		//!Begin the * more important then +
		//! 1 + 2 * 3 -> create first the left node begin the 1 and verify the right node wil be (2 * 3)
		right = parse_multiplication(str);
		if (!right)
		{
			destroy_tree(left);
			return (NULL);
		}
		//?Create SUM node
		tmp.type = ADD;
		tmp.l = left;
		tmp.r = right;
		tmp.val = 0; // !the add node has no value
		left = new_node(tmp); //? make it begin the heap
		if (!left)
		{
			destroy_tree(right);
			return (NULL);
		}
	}
	return (left);
}

//?This function is the initial part of the new code
node    *parse_expr(char **s)
{
    node	*ret = parse_addtion(s);

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

/*
Example of how the AST must be make;
INPUT: "3+4*5"
AST:
      +
     / \
    3   *
       / \
      4   5
RESOLUTION: 3 + (4 * 5) = 3 + 20 = 23
*/


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
	char		*str;
	if (argc != 2) //? Only a single string is accept
		return (1);
	str = argv[1];
    node *tree = parse_expr(&str);
    if (!tree)
        return (1);
    printf("%d\n", eval_tree(tree));
    destroy_tree(tree);
}