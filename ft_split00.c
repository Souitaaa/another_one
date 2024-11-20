/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split00.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csouita <csouita@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 18:20:32 by csouita           #+#    #+#             */
/*   Updated: 2024/11/19 18:21:29 by csouita          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	words_nbr00(char *str)
{
	char	c;

	int (i), (words_count), (start);
	i = 0;
	words_count = 0;
	while (str[i])
	{
		while (str[i] && ft_is_void(str[i]))
			i++;
		start = i;
		while (str[i] && !ft_is_void(str[i]))
		{
			if (str[i] && (str[i] == '\'' || str[i] == '"'))
			{
				words_count++;
				c = str[i];
				i++;
				while (str[i] && str[i] != c)
					i++;
				if (str[i] && str[i] == c)
					i++;
			}
			else
				i++;
		}
		if (start != i)
			words_count++;
		if (str[i] == '\0')
			return (words_count);
		i++;
	}
	return (words_count);
}

static char	*malloc_word00(char *s, int begin, int end)
{
	int		i;
	char	*str;

	str = (char *)gc_malloc((end - begin + 1) * sizeof(char));
	if (!str)
		return (NULL);
	i = 0;
	while (begin < end)
	{
		str[i] = s[begin];
		i++;
		begin++;
	}
	str[i] = '\0';
	return (str);
}

static char	**freefun00(char **p, int j)
{
	while (j >= 0)
	{
		gc_remove_ptr(p[j]);
		j--;
	}
	gc_remove_ptr(p);
	return (NULL);
}

static char	**list00(char **lst, char *s)
{
	char	c;

	int (i), (j), (start);
	i = 0;
	j = 0;
	start = 0;
	while (s[i])
	{
		if (!ft_is_void(s[i]))
		{
			start = i;
			while (s[i] && !ft_is_void(s[i]))
			{
				if (s[i] && (s[i] == '\'' || s[i] == '"'))
				{
					c = s[i];
					i++;
					while (s[i] && s[i] != c)
						i++;
					if (s[i] && s[i] == c)
						i++;
				}
				else
					i++;
			}
			lst[j] = malloc_word00(s, start, i);
			if (!lst[j])
				return (freefun00(lst, j));
			j++;
		}
		else
			i++;
	}
	lst[j] = NULL;
	return (lst);
}

char	**ft_split00(char *s)
{
	char	**lst;

	if (!s)
		return (NULL);
	lst = (char **)gc_malloc((words_nbr00(s) + 1) * sizeof(char *));
	if (!lst)
		return (NULL);
	return (list00(lst, s));
}
