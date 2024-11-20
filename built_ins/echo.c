/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboumlik <mboumlik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 09:49:04 by mboumlik          #+#    #+#             */
/*   Updated: 2024/11/13 04:27:33 by mboumlik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int all_n(char *s)
{
    int i;

    if (ft_strlen(s) <= 1)
        return (0);
    if (s[0] != '-')
        return (0);
    i = 1;
    while (s[i])
    {
        if (s[i] != 'n')
            return (0);
        i++;
    }
    return (1);
}

int echo_(char **args)
{
    int i = 1;
    int newline = 1;
    if (args[1] && all_n(args[1])) {
        newline = 0;
        i++;
    }
    while (args[i]) {
        ft_putstr_fd(args[i], 1);
        if (args[i + 1])
            ft_putchar_fd(' ', 1);
        i++;
    }
    if (newline)
        ft_putchar_fd('\n', 1);
    return 0;
}
