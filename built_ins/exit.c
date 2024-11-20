/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csouita <csouita@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 09:51:37 by mboumlik          #+#    #+#             */
/*   Updated: 2024/11/14 17:39:32 by csouita          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int exit_(char **args)
{
    int exit_status = 0;
    int j = 0;
    long val = ft_atoi(args[0]);
    ft_putstr_fd("exit\n", 2);
    if (!args || !*args)
        exit(0);
    if (args[0][j] == '-' || args[0][j] == '+')
        j++;
    while (args[0] && args[0][j])
    {
        if (!ft_isdigit(args[0][j]))
        {
            printf("bash: exit: %s: numeric argument required\n",args[0]);
            exit(2);
        }
        j++;
    }
    if(args && args[0] && args[1])
    {
        printf("bash: exit: too many arguments\n");
        g_global.last_exit_status = 1;
        return g_global.last_exit_status;
    }
    if ((val > 0 && args[0][0] == '-') || (val < 0 && args[0][0] != '-') || ft_strlen(args[0]) > 20)
    {
        printf("bash: exit: %s: numeric argument required\n",args[0]);
        exit(2);
    }
    exit_status = ft_atoi(args[0]) % 256;
    exit(exit_status);
}
