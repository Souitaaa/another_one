/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csouita <csouita@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 18:52:46 by csouita           #+#    #+#             */
/*   Updated: 2024/11/14 18:42:11 by csouita          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_spaces(t_data *data)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(data->line);
	while (i < len)
	{
		while (data->line[i] == 32)
			i++;
		if ((data->line[i] == '>' && data->line[i + 1] == '>')
			|| (data->line[i] == '<' && data->line[i + 1] == '<'))
		{
			write(1, " ", 1);
			write(1, &data->line[i], 2);
			write(1, " ", 1);
			i += 2;
		}
		else if (data->line[i] == '>' || data->line[i] == '<'
			|| data->line[i] == '|')
		{
			write(1, " ", 1);
			write(1, &data->line[i], 1);
			write(1, " ", 1);
			i++;
		}
		else
		{
			if (i > 0 && (data->line[i - 1] == '>' || data->line[i - 1] == '<'
					|| data->line[i - 1] == '|') && data->line[i] == 32)
				write(1, &data->line[i], 1);
			else
				write(1, &data->line[i], 1);
			i++;
		}
	}
}

void	ft_lstadd_back_lexer(t_lexer **lst, t_lexer *new)
{
	t_lexer	*p;

	p = *lst;
	if (lst == NULL || new == NULL)
	{
		return ;
	}
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	while (p->next != NULL)
	{
		p = p->next;
	}
	p->next = new;
}

t_lexer	*new_lexer_node(t_tokens type, char *str)
{
	t_lexer	*new_node;

	new_node = (t_lexer *)gc_malloc(sizeof(t_lexer));
	if (!new_node)
		return (NULL);
	new_node->str = str;
	new_node->tokens = type;
	new_node->next = NULL;
	return (new_node);
}

int quoted_string(char *str)
{
	int i = 0 ;
	char  c;
	if(!str)
		return 0;
	while(str[i])
	{
		if(str[i] == '\'' || str[i] == '\"')
			c = str[i];
		if(str[i] == c && str[i + 1] == c)
			return 1;
		i++;
	}
	return 0;
}

void	add_node(t_lexer **head, t_tokens type, char *str)
{
	t_lexer	*node;
	if(!str)
		return;
	// if (quoted_string(str))
	// 	return;
	node = new_lexer_node(type, str);
	ft_lstadd_back_lexer(head, node);
}
