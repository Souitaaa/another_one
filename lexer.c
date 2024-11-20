/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboumlik <mboumlik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 20:13:52 by csouita           #+#    #+#             */
/*   Updated: 2024/11/19 12:02:51 by mboumlik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	word(t_data *data, int i)
{
	int		start;
	char	*line;

	line = NULL;
	start = i;
	if (data->line[i] == '"')
	{
		i++;
		while (data->line[i] && data->line[i] != '"')
			i++;
		if (data->line[i])
			i++;
	}
	else if (data->line[i] && data->line[i] == '\'')
	{
		i++;
		while (data->line[i] && data->line[i] != '\'')
			i++;
		if (data->line[i])
			i++;
	}
	else
	{
		while (data->line[i] && data->line[i] != '"' && data->line[i] != '\''
			&& data->line[i] != '>' && data->line[i] != '<'
			&& data->line[i] != '|' && data->line[i] != 32)
			i++;
	}
	line = ft_substr(data->line, start, i - start);
	add_node(&data->head, WORD, line);
	return (i);
}

int	add_whitespace(int i, t_data **data)
{
	add_node(&(*data)->head, WHITESPACE, " ");
	while ((*data)->line[i] && ((*data)->line[i] == ' '
			|| (*data)->line[i] == '\t'))
		i++;
	if (!(*data)->line[i])
		return (-1);
	return (i);
}

void	lexer(t_data *data)
{
	int	i;

	i = 0;
	data->head = NULL;
	while (data->line[i])
	{
		if (data->line[i] == ' ' || data->line[i] == '\t')
		{
			i = add_whitespace(i, &data);
			if (i == -1)
				break ;
		}
		if (data->line[i] == '|')
		{
			add_node(&data->head, PIPE, "|");
			i++;
		}
		else if (data->line[i] == '<' && data->line[i + 1] == '<')
		{
			add_node(&data->head, HEREDOC, "<<");
			i = i + 2;
		}
		else if (data->line[i] == '>' && data->line[i + 1] == '>')
		{
			add_node(&data->head, APPEND, ">>");
			i = i + 2;
		}
		else if (data->line[i] == '<')
		{
			add_node(&data->head, REDIR_IN, "<");
			i++;
		}
		else if (data->line[i] == '>')
		{
			add_node(&data->head, REDIR_OUT, ">");
			i++;
		}
		else
			i = word(data, i);
	}
}

int	check_redirection(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '>' || line[i] == '<')
		{
			while (line[i] == '>' || line[i] == '<')
				i++;
			while (line[i] == ' ' || line[i] == '\t')
				i++;
			if (line[i] == '\0' || line[i] == '|')
				return (0);
		}
		i++;
	}
	return (1);
}
int chech_spaces_after_pipe(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '|')
		{
			i++;
			while (line[i] == ' ' || line[i] == '\t')
				i++;
			if (line[i] == '\0')
				return (0);
		}
		i++;
	}
	return (1);
}

void handle_signals(int sig)
{
	if (sig == in_heredoc)
	{
		signal(SIGINT,SIG_DFL);
		signal(SIGQUIT,SIG_IGN);	
	}
	if (sig == in_child)
	{
		signal(SIGINT,SIG_DFL);
		signal(SIGQUIT,SIG_DFL);
	}
	if (sig == in_parent)
	{
		signal(SIGINT,SIG_IGN);
		signal(SIGQUIT,SIG_IGN);
		
	}
	if (sig == before_readline)
	{
		signal(SIGINT,handle_sigint);
		signal(SIGQUIT,SIG_IGN);
	}
}
int consecutive_redirections(char *line)
{
	int	i = 0;
	if (!line)
		return 1;
	while (line[i])
	{
		if (line[i] == '>' && line[i + 1] == '<')
			return 0;
		if (line[i] == '<' || line[i] == '>')
		{
			int redir_count = 0;
			while (line[i] == '<' || line[i] == '>')
			{
				redir_count++;
				i++;
			}
			if (redir_count > 2)
				return 0;
			while (line[i] && is_whites_spaces(line[i]))
				i++;
			if (line[i] == '<' || line[i] == '>')
				return 0;
		}
		else
			i++;
	}
	return 1;
}

int	syntax_error(t_data *data)
{
	if (!data->line[0])
		return (1);
	if (chech_spaces_after_pipe(data->line) == 0)
		return (0);
	if (data->line[0] == '|' || data->line[ft_strlen(data->line) - 1] == '|'
		|| check_pipe(data->line))
		return (0);
	if (data->line[ft_strlen(data->line)] == '>'
		|| data->line[ft_strlen(data->line)] == '<')
		return (0);
	if (check_redirection(data->line) == 0)
		return (0);
	if (consecutive_redirections(data->line) == 0)
		return 0;
	if (check_quotes(data) == 0)
		return (0);
	return (1);
}
