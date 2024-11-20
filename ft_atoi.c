/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboumlik <mboumlik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 16:19:06 by mboumlik          #+#    #+#             */
/*   Updated: 2024/09/06 09:34:29 by mboumlik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

long ft_atoi(const char *str)
{
	long	j;
	int					i;
	int					sign;
	
	if (str == NULL)
        return 0;

	i = 0;
	j = 0;
	sign = 1;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while ((str[i] >= '0' && str[i] <= '9'))
	{
		j = j * 10 + str[i] - '0';
		i++;
	}
	return (j * sign);
}
