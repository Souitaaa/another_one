/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboumlik <mboumlik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 09:50:36 by mboumlik          #+#    #+#             */
/*   Updated: 2024/10/03 11:27:32 by mboumlik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int pwd_(t_env *env)
{
    char cwd[PATH_MAX];
    char *pwd_value;

    // Try getting the current directory
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        ft_putstr_fd(cwd, 1);
        ft_putstr_fd("\n", 1);
        return 0;
    }
    
    // If getcwd fails, try getting it from the environment
    pwd_value = get_env_value(env, "PWD");
    if (pwd_value)
    {
        ft_putstr_fd(pwd_value, 1);
        ft_putstr_fd("\n", 1);
        return 0;
    }

    // If both methods fail
    ft_putstr_fd("pwd: error retrieving current directory\n", 2);
    return 1;
}
