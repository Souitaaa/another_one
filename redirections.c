/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboumlik <mboumlik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 04:11:51 by mboumlik          #+#    #+#             */
/*   Updated: 2024/11/14 04:15:35 by mboumlik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int redirect_out(char *file_name)
{
    int fd;
    fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
    {
        printf("minishell : %s : permission denied\n",file_name);
        return 1;
    }
    dup2(fd, STDOUT_FILENO);
    close(fd);
    return 0;
}

int redirect_appand(char *file_name)
{
    int fd = open(file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd == -1)
    {
        printf("minishell : %s : permission denied\n",file_name);                    
        return 1;
    }
    dup2(fd, STDOUT_FILENO);
    close(fd);
    return 0;
}

int redirect_in(char *file_name)
{
    int fd = open(file_name, O_RDONLY);
    if (fd == -1)
    {
        printf("minishell : %s : permission denied\n",file_name);                    
        return 1;
    }
    dup2(fd, STDIN_FILENO);
    close(fd);
    return 0;
}

int handle_redirections(t_command *command)
{
    t_file *current = command->file;
    int ret = 0;
    while (current)
    {
        if (current->is_ambigous)
        {
            ft_putstr_fd("minishell: ambiguous redirect\n", 2);
            ret = 1;
            break;
        }
        if (current->file_type == REDIR_OUT)
            ret = redirect_out(current->file_name);
        else if (current->file_type == REDIR_IN)
            ret =  redirect_in(current->file_name);
        else if (current->file_type == APPEND)
            ret =  redirect_appand(current->file_name); 
        if (ret == 1)
            return (1);
        current = current->next;
    }
    return ret;
}
