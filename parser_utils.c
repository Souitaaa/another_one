/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csouita <csouita@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 19:58:28 by csouita           #+#    #+#             */
/*   Updated: 2024/11/14 18:46:50 by csouita          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_file	*ft_create_node(char *file_name, t_tokens redirection_type,
		int is_ambigous, int is_qouted)
{
	t_file	*node;
	char	*unquoted_name;

	node = (t_file*)gc_malloc(sizeof(t_file));
	unquoted_name = handle_quotes(file_name);
	if (is_ambigous)
		node->is_ambigous = 1;
	else
		node->is_ambigous = 0;
	if (is_qouted)
	{
		node->is_qouted = 1;
	}
	else
		node->is_qouted = 0;
	node->file_name = unquoted_name;
	node->file_type = redirection_type;
	node->next = NULL;
	return (node);
}

void	ft_unmask(char **str)
{
	int	i;

	i = 0;
	while ((*str)[i])
	{
		if ((*str)[i] == 6)
			(*str)[i] = '\'';
		i++;
	}
}

t_command	*ft_add_command(char *command, t_file *file)
{
	t_command	*node;
	int			i;
	char		**commands = NULL;

	i = 0;
	commands = ft_split00(command); 
	if (!commands)
	{
		node =(t_command *)gc_malloc(sizeof(t_command));
		node->cmd = NULL;
		node->file = file;
		node->next = NULL;
		return (node);
	}
	node = (t_command *)gc_malloc(sizeof(t_command));
	while (commands[i])
		i++;
	node->cmd = (char **)gc_malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (commands[i])
	{
		node->cmd[i] = handle_quotes(commands[i]);
		ft_unmask(&node->cmd[i]);
		i++;
	}
	node->cmd[i] = NULL;
	node->file = file;
	node->next = NULL;
	return (node);
}

void	ft_create_command(t_command **command_list, char *command,
		t_file **file)
{
	t_command	*node;

	
	node = ft_add_command(command, (*file));
	ft_lstadd_back_command(command_list, node);
}
