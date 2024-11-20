/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csouita <csouita@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 20:37:02 by csouita           #+#    #+#             */
/*   Updated: 2024/11/14 18:48:07 by csouita          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strdup(char *s1)
{
	int		i;
	char	*dest;

	if (!s1)
		return (NULL);
	i = 0;
	dest = (char *)gc_malloc(sizeof(char) * (ft_strlen(s1) + 1));
	if (!dest)
		return (NULL);
	while (s1[i])
	{
		dest[i] = s1[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*ft_strchr(char *s, int c)
{
	int		i;
	char	*str;

	i = 0;
	str = (char *)s;
	while (s[i] != (char)c)
	{
		if (str[i] == '\0')
			return (NULL);
		i++;
	}
	return (str + i);
}

int	ft_strchr_(char *s, int c)
{
	int		i;
	char	*str;

	i = 0;
	str = (char *)s;
	while (s[i] != (char)c)
	{
		if (str[i] == '\0')
			return (0);
		i++;
	}
	return (1);
}

int	has_quote(char *delim)
{
	int	i;

	if (!delim)
		return (0);
	i = 0;
	while (delim[i])
	{
		if (delim[i] && (delim[i] == '\'' || delim[i] == '"'))
			return (1);
		i++;
	}
	return (0);
}

int	ft_is_void(char c)
{
	return (c == 32 || (c >= 9 && c <= 13));
}


void	*ft_memcpy(void *dest,void *src, size_t n)
{
	size_t	i;

	i = 0;
	if (dest == src)
		return (dest);
	if (!dest && !src)
		return (NULL);
	while (i < n)
	{
		((unsigned char *)dest)[i] = ((unsigned char *)src)[i];
		i++;
	}
	return (dest);
}
