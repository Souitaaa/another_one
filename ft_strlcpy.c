/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboumlik <mboumlik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 18:02:58 by mboumlik          #+#    #+#             */
/*   Updated: 2023/12/04 20:58:17 by mboumlik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./minishell.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t n)
{
	size_t	srcsize;
	size_t	i;

	srcsize = ft_strlen((char *)src);
	i = 0;
	if (n != 0)
	{
		while (src[i] != '\0' && i < (n - 1))
		{
			dst[i] = src[i];
			i++;
		}
		dst[i] = '\0';
	}
	return (srcsize);
}
