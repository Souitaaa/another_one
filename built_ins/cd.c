/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csouita <csouita@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 11:33:25 by mboumlik          #+#    #+#             */
/*   Updated: 2024/11/19 18:24:12 by csouita          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void update_env_var(t_env *env, char *key, char *value)
{
    t_env *current;
    t_env *new_node;
    
    if (!env || !key || !value)
        return;
    current = env;
    while (current)
    {
        if (current->key && ft_strncmp(current->key, key,-1) == 0)
        {
            if (current->value)
                gc_remove_ptr(current->value);
            current->value = ft_strdup(value);
            return;
        }
        current = current->next;
    }
    new_node = gc_malloc(sizeof(t_env));
    if (!new_node)
        return;
    new_node->key = ft_strdup(key);
    new_node->value = ft_strdup(value);
    new_node->next = env;
    env = new_node;
}

char *get_env_value(t_env *env, const char *key)
{
    t_env *current;
    
    if (!env || !key)
        return NULL;
    current = env;
    while (current)
    {
        if (current->key && ft_strncmp(current->key, key,-1) == 0)
            return current->value;
        current = current->next;
    }
    return NULL;
}

t_env* get_node_by_key_from_env(t_env *env, char *key)
{
    t_env *current;
    
    if (!env || !key)
        return NULL;
    current = env;
    while (current)
    {
        if (current->key && ft_strcmp(current->key, key) == 0)
            return current;
        current = current->next;
    }
    return NULL;
}

int cd_(char **args, t_env *env)
{
    char *path;
    char cwd[PATH_MAX];
    char old_cwd[PATH_MAX];
    char new_path[PATH_MAX];    

    if (!env)
        return 1;

    if (getcwd(old_cwd, sizeof(old_cwd)) == NULL)
        update_env_var(env, "OLDPWD", old_cwd);
    if (!args[1] || args[1][0] == '\0')
    {
        path = get_env_value(env, "HOME");
        if (!path)
        {
            ft_putstr_fd("cd: HOME not set\n", 2);
            return 1;
        }
    }
    else
        path = args[1];

    // Handle ~ as home directory
    if (path[0] == '~' && (path[1] == '/' || path[1] == '\0'))
    {
        char *home = get_env_value(env, "HOME");
        if (!home)
        {
            ft_putstr_fd("cd: HOME not set\n", 2);
            return 1;
        }
        ft_strlcpy(new_path, home, PATH_MAX);
        ft_strlcat(new_path, path + 1, PATH_MAX);
        path = new_path;
    }
    // Try changing the directory
    if (chdir(path) != 0)
    {
        ft_putstr_fd("cd: ", 2);
        ft_putstr_fd(path, 2);
        ft_putstr_fd(": ", 2);
        ft_putstr_fd(strerror(errno), 2);
        ft_putstr_fd("\n", 2);
        return 1;
    }
    // Update PWD and OLDPWD environment variables
    if (getcwd(cwd, sizeof(cwd)) != NULL)
        update_env_var(env, "PWD", cwd);
    else
    {
        ft_putstr_fd("cd: Error retrieving new current directory\n", 2);
        return 1;
    }

    return 0;
}
