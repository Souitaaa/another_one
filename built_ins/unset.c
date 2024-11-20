/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csouita <csouita@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 12:20:29 by mboumlik          #+#    #+#             */
/*   Updated: 2024/11/19 18:24:37 by csouita          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int is_valid_env_name(char *str)
{
    int i;

    if (!str || !str[0] || ft_isdigit(str[0]))
        return (0);
    i = 0;
    while (str[i])
    {
        if (!ft_isalnum(str[i]) && str[i] != '_')
            return (0);
        i++;
    }
    return (1);
}

void remove_env_var(t_env **env, char *key, t_variables vars)
{
    t_env *current;
    t_env *prev;
    if (!env || !*env || !key)
        return;
    
    current = *env;
    prev = NULL;
    if (current && !ft_strncmp(current->key, key, ft_strlen(key) + 1))
    {
        *(vars.env) = current->next;
        gc_remove_ptr(current->key);
        gc_remove_ptr(current->value);
        gc_remove_ptr(current);
        return;
    }
    while (current)
    {
        if (!ft_strncmp(current->key, key,ft_strlen(key)+1))
        {
            prev->next = current->next;
            gc_remove_ptr(current->key);
            gc_remove_ptr(current->value);
            gc_remove_ptr(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}

int unset_(char **args, t_env **env, t_variables vars)
{
    int i;
    int status;

    if (!args || !args[1])
        return (0);  // No arguments, successfully do nothing
    
    status = 0;
    i = 1;
    while (args[i])
    {
        if (!is_valid_env_name(args[i]))
        {
            ft_putstr_fd("minishell: unset: `", 2);
            ft_putstr_fd(args[i], 2);
            ft_putstr_fd("': not a valid identifier\n", 2);
            status = 1;
        }
        else
            remove_env_var(env, args[i], vars);
        i++;
    }
    return (status);
}
