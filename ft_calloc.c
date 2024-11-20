/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csouita <csouita@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 05:06:17 by mboumlik          #+#    #+#             */
/*   Updated: 2024/11/14 18:42:53 by csouita          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void	ft_bzero(void *s, size_t n)
{
	unsigned char	*vd;
	size_t			i;

	vd = (unsigned char *)s;
	i = 0;
	while (i < n)
	{
		vd[i] = 0;
		i++;
	}
}
void	*ft_calloc(size_t count, size_t size)
{
	size_t	i;
	void	*x;

	if (size && count > __SIZE_MAX__ / size)
		return (NULL);
	i = count * size;
	x = gc_malloc(i);
	if (!x)
		return (NULL);
	ft_bzero(x, i);
	return (x);
}


