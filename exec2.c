/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csouita <csouita@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 15:16:27 by mboumlik          #+#    #+#             */
/*   Updated: 2024/11/19 18:37:17 by csouita          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./minishell.h"

// Helper function to convert env list to array for execve
void to_arr(char **env_array,t_env *temp)
{
    int i = 0;

    while (temp)
    {
        size_t len = ft_strlen(temp->key) + ft_strlen(temp->value) + 2;
        env_array[i] = gc_malloc(len);
        if (!env_array[i])
        {
            while (--i >= 0)
                gc_remove_ptr(env_array[i]);
            gc_remove_ptr(env_array);
            return ;
        }
        ft_strlcpy(env_array[i], temp->key, len);
        ft_strlcat(env_array[i], "=", len);
        ft_strlcat(env_array[i], temp->value, len);
        
        i++;
        temp = temp->next;
    }
}
char **env_list_to_array(t_env *env)
{
    int count = 0;
    t_env *temp = env;
    while (temp)
    {
        count++;
        temp = temp->next;
    }
    char **env_array = gc_malloc(sizeof(char *) * (count + 1));
    if (!env_array)
        return NULL;
    temp = env;
    to_arr(env_array,temp);
    env_array[count] = NULL;
    return env_array;
}

void free_env_array(char **env_array)
{
    if (!env_array)
        return;
    for (int i = 0; env_array[i]; i++)
        gc_remove_ptr(env_array[i]);
    gc_remove_ptr(env_array);
}
