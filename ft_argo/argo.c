/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsilva-x <lsilva-x@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 18:37:30 by lsilva-x          #+#    #+#             */
/*   Updated: 2025/07/24 20:11:25 by lsilva-x         ###   ########.fr       */
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

*/