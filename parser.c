/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csouita <csouita@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 17:01:46 by csouita           #+#    #+#             */
/*   Updated: 2024/11/14 18:47:30 by csouita          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void	handle_word(char **command, t_lexer **head)
{
	char	*temp;

	temp = ft_strjoin(*command, (*head)->str);
	*command = temp;
	if ((*head)->next && (*head)->next->tokens == WHITESPACE)
	{
		temp = ft_strjoin(*command, " ");
		*command = temp;
	}
	*head = (*head)->next;
}

void	ft_mask(char *c)
{
	if (*c == '\'')
		*c = 6;
}
char	*remove_escape_characters(char *str)
{
	char	*new_str;
	int		i;
	int		j;

	i = 0;
	j = 0;
	new_str = (char *)gc_malloc(ft_strlen(str) + 1);
	while (str[i])
	{
		if (str[i] == 1)
			i++;
		new_str[j] = str[i];
		i++;
		j++;
	}
	new_str[j] = '\0';
	return (new_str);
}

char	*handle_quotes(char *arg)
{
	char	quote_char;
	char	*str;

	int (i), (j);
	i = 0;
	j = 0;
	str =(char *)gc_malloc(ft_strlen(arg) + 1);
	while (arg[i])
	{
		if ((arg[i] == '\"' || arg[i] == '\'')
			&& (i == 0 || (i != 0 && arg[i - 1] != 1)))
		{
			quote_char = arg[i++];
			while (arg[i] && (arg[i] != quote_char || (arg[i] == quote_char
						&& arg[i - 1] == 1)))
				str[j++] = arg[i++];
			if (arg[i] == quote_char)
				i++;
		}
		else
			str[j++] = arg[i++];
	}
	str[j] = '\0';
	return (remove_escape_characters(str));
}

int	is_redirection(t_tokens token)
{
	return (token == REDIR_IN || token == REDIR_OUT || token == HEREDOC
		|| token == APPEND);
}

void	parser_works(char **command, t_lexer **head, t_file **file_name)
{
	if (*head == NULL)
		return ;
	if ((*head)->tokens == WORD)
		handle_word(command, head);
	else if (is_redirection((*head)->tokens))
		handle_redirection(head, file_name, (*head)->tokens);
	else
		*head = (*head)->next;
}

t_command	*parser(t_data *data)
{
	t_file		*file;
	t_lexer		*head;
	char		*command;
	t_command	*command_list;
	// t_file		*temp;

	file = NULL;
	head = data->head;
	command = NULL;
	command_list = NULL;
	while (head)
	{
		while (head && head->tokens != PIPE)
			parser_works(&command, &head, &file);
		ft_create_command(&command_list, command, &file);
		command = NULL;
		file = NULL;
		if (head && head->tokens == PIPE)
			head = head->next;
	}
	return (command_list);
}
