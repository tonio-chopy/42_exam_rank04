# FT_VBC
VBC = very Basic Compiler

## Subject
The program must behavior as a calculator, accept only operations of sum and multiply. it must can handle parenthesis. all value will be between 0 a 9

The main code will be parser the values and mount a binary tree to correctly handle the parenthesis to do the maht in the correct order;

## Concptes

### Left-associative
Conceito utilizado para ordem de resolução matemática e também utilizado para a construção da AST

1 + 2 +3 é tratado como (1 + 2) + 3

### 
A arvore é construida de forma que cada nó abrace o último nó criado, formando a AST

Ex: 1 + 2 + 3

1 + 2:
      +
     / \
    1   2

Criação do primeiro nó com leaf para dirieta e para esquerda

.. + 3 
        +
       / \
         3
O segundo nó já se conecta de forma que o nó anterior seja o seu a esquerda formando a AST completa e utilizando a resolução de _Left-associative_ conseguimos calcular de forma correta
```txt
        +
       / \
      +   3
     / \
    1   2
```

## Error handler
In case of unexpected symbol or inappropriate parenthesis
_"Unexpected token '%c'\n"_ -> exit with code status 1

if the
symbol is the end of input you will print: _"Unexpected end of input\n"_

## Acronym
ret -> return (?maybe)
AST -> Abystract Syntax Tree
## Given Code

### Struct

#### Main struct
```c
typedef struct node {
    enum {
        ADD,
        MULTI,
        VAL
    }   type;
    int val;
    struct node *l;
    struct node *r;
```
*type* ->  type is a enum type that can only accept the previous enum set values
*val* -> the current node value
*struct node *l* -> Left node
*struct node *r* -> Right node

#### Recursively destroy node function
this condition referest to the fact of VAl are leaf nodes and they don't had childrens
```c
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
```
*destroy_tree(n->l)* -> Recursively destroy the left nodes
*destroy_tree(n->r)* -> Recursively destroy the right nodes

#### parse_addtion(chr *s)
A primeira função principal que começa a recursão juntamente com o tratamento das outras operações parenteses e erro


## Code Workflow
O código funciona por camádas e nó, sendo primeiro feito uma verificação crucial se o primeiro caractere é um primary ou seja número ou parenteses.
número -> node do tipo VAL (leaf)
parenteses -> chama parse_addtion novamente criando mais uma camada

se for multiplicação o mesmo continua na mesma camada gerando o node da direita.

se não for ele volta para parse_addtion gerando o nó de soma e criando o membro da direita, o mesmo ao mesmo tempo já verificado se o próximo é uma multiplicação caso sim ele cria uma novacamada fazendo uma verificação igual