/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csouita <csouita@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 18:54:47 by csouita           #+#    #+#             */
/*   Updated: 2024/11/07 19:38:39 by csouita          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_whites_spaces(char c)
{
	return (c == 32 || (c >= 9 && c <= 13));
}

int	check_pipe(char *s)
{
	int	i;

	if (!s)
		return (1);
	i = 0;
	while (s[i] && is_whites_spaces(s[i]))
		i++;
	while (s[i])
	{
		if (s[i] && s[i] == '|')
		{
			if (s[i])
				i++;
			while (s[i] && is_whites_spaces(s[i]))
				i++;
			if (s[i] && s[i] == '|')
				return (1);
		}
		i++;
	}
	return (0);
}

int	check_quotes(t_data *data)
{
	int	i;
	int	quote_char;

	i = 0;
	quote_char = 0;
	while (data->line[i])
	{
		while (data->line[i] && (data->line[i] != '\''
				&& data->line[i] != '\"'))
			i++;
		if (!data->line[i])
			return (1);
		quote_char = data->line[i];
		i++;
		while (data->line[i] && data->line[i] != quote_char)
			i++;
		if (!data->line[i])
			return (0);
		i++;
	}
	return (1);
}
