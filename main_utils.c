/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csouita <csouita@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 19:52:17 by csouita           #+#    #+#             */
/*   Updated: 2024/11/14 18:45:06 by csouita          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*create_node(char *env)
{
	int		i;
	t_env	*lst;
	char	*key;
	char	*value;

	i = 0;
	lst = (t_env *)gc_malloc(sizeof(t_env));
	key = ft_substr(&env[i], 0, search_in(env, '='));
	lst->key = key;
	value = ft_substr(&env[i], search_in(env, '=') + 1, ft_strlen(env));
	lst->value = value;
	lst->next = NULL;
	return (lst);
}

t_env	*get_last_node(t_env *head)
{
	if (!head)
		return (NULL);
	while (head->next)
		head = head->next;
	return (head);
}
