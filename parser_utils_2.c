/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboumlik <mboumlik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 19:59:47 by csouita           #+#    #+#             */
/*   Updated: 2024/11/08 21:09:42 by mboumlik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void	handle_redirection(t_lexer **head, t_file **file_name,
		int redirection_type)
{
	t_file	*node;
	char	*temp;
	char	**file_name_tmp;
	int		is_qouted;
	int		is_ambigous;

	temp = NULL;
	file_name_tmp = NULL;
	is_qouted = 0;
	is_ambigous = 0;
	// printf("head->str == %s\n",(*head)->str);
	*head = (*head)->next;
	while (*head && (*head)->tokens == WHITESPACE)
		*head = (*head)->next;
	while (*head && (*head)->tokens != WORD)
		*head = (*head)->next;
	while (*head && (*head)->tokens == WORD)
	{
		// printf("str == %s\n",(*head)->str);
		temp = ft_strjoin(temp, (*head)->str);
		*head = (*head)->next;
	}
	// printf("temp == %s\n",temp);
	// int c = check_qoutes2(temp);
	// printf("checkk === %d\n",c);
	if (check_qoutes2(temp))
		is_qouted = 1;
	else
	{
		file_name_tmp = ft_split00(temp);
		if (count_arg(file_name_tmp) != 1)
			is_ambigous = 1;
		else
			temp = file_name_tmp[0];
	}
	node = ft_create_node(temp, redirection_type, is_ambigous, is_qouted);
	ft_lstadd_back_file(file_name, node);
}

int	count_arg(char **str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

int	check_qoutes2(char *str)
{
	int i = 0;
	// printf("str lakhera ==== %s\n",str);
	while(str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
			return (1);	
		i++;
	}
	return (0);
}
