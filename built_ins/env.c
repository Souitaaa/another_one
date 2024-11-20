/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboumlik <mboumlik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 08:29:48 by mboumlik          #+#    #+#             */
/*   Updated: 2024/11/13 01:30:03 by mboumlik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


int env_(char **args,t_env *env)
{
    if (!args[1])
    {
        t_env *current = env;
        while (current != NULL)
        {
            printf("%s=%s\n", current->key, current->value);
            current = current->next;
        }
    }
    else
    {
        ft_putstr_fd("env: '", 2);
        ft_putstr_fd(args[1], 2);
        ft_putstr_fd("': No such file or directory\n", 2);
        return 127;
    }
    return 0;
}
