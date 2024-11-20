/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboumlik <mboumlik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 18:02:20 by mboumlik          #+#    #+#             */
/*   Updated: 2024/10/25 13:07:38 by mboumlik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./minishell.h"

size_t	ft_strlcat(char *dest,char *src, size_t n)
{
	size_t	len_s;
	size_t	len_d;
	size_t	i;
	size_t	sum;

	if (!dest && !n)
		return (ft_strlen(src));
	len_s = ft_strlen(src);
	len_d = ft_strlen(dest);
	i = 0;
	if (len_d > n)
		sum = len_s + n;
	else
		sum = len_s + len_d;
	if (n == 0)
		return (sum);
	while (src[i] && i + len_d < n - 1)
	{
		dest[len_d + i] = src[i];
		i++;
	}
	dest[len_d + i] = '\0';
	return (sum);
}
