/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csouita <csouita@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 05:09:35 by mboumlik          #+#    #+#             */
/*   Updated: 2024/11/19 18:21:09 by csouita          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

char *check_local_command(char *cmd)
{
    if (cmd[0] == '/' || ft_strncmp(cmd, "./", 2) == 0 || ft_strncmp(cmd, "../", 3) == 0)
    {
        if (access(cmd, X_OK) == 0)
            return ft_strdup(cmd);
        return NULL;
    }
    char *local_path = ft_calloc(ft_strlen(cmd) + 3, sizeof(char));
    if (!local_path)
        return NULL;
        
    ft_strlcpy(local_path, "./", 3);
    ft_strlcat(local_path, cmd, ft_strlen(cmd) + 3);
    
    if (access(local_path, X_OK) == 0)
        return local_path;
    
    gc_remove_ptr(local_path);
    return NULL;
}
char *find_path_env(t_env *env)
{
    t_env *path_var = env;
    char *path = NULL;
    while (path_var)
    {
        if (ft_strncmp(path_var->key, "PATH", 4) == 0)
        {
            path = path_var->value;
            break;
        }
        path_var = path_var->next;
    }
    return path;
}
char *construct_cmd_path(char *dir_path, char *cmd)
{
    char *full_path;

    full_path = ft_calloc(ft_strlen(dir_path) + ft_strlen(cmd) + 2, sizeof(char));
    if (!full_path)
        return NULL;

    ft_strlcpy(full_path, dir_path, ft_strlen(dir_path) + 1);
    ft_strlcat(full_path, "/", ft_strlen(dir_path) + 2);
    ft_strlcat(full_path, cmd, ft_strlen(dir_path) + ft_strlen(cmd) + 2);

    return full_path;
}

char *search_path_directory(char *current, char *cmd, char **next)
{
    char *full_path;
    
    *next = ft_strchr(current, ':');
    if (*next)
        **next = '\0';

    full_path = construct_cmd_path(current, cmd);
    if (!full_path)
        return NULL;

    if (access(full_path, X_OK) == 0)
        return full_path;

    gc_remove_ptr(full_path);
    return NULL;
}

char *search_in_path_dirs(char *path_copy, char *cmd)
{
    char *result = NULL;
    char *next = NULL;
    char *current = path_copy;
    
    while (current && *current)
    {
        result = search_path_directory(current, cmd, &next);
        if (result)
            break;
            
        if (next)
            current = next + 1;
        else
            current = NULL;
    }

    gc_remove_ptr(path_copy);
    return result;
}

char *find_command(char *cmd, t_env *env)
{
    char *local_result = check_local_command(cmd);
    if (local_result)
        return local_result;
    char *path = find_path_env(env);
    if (!path)
        return NULL;
    char *path_copy = ft_strdup(path);
    if (!path_copy)
        return NULL;

    return search_in_path_dirs(path_copy, cmd);
}
