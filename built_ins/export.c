/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csouita <csouita@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 11:04:34 by mboumlik          #+#    #+#             */
/*   Updated: 2024/11/19 18:24:28 by csouita          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int count_env_vars(t_env *env)
{
    int count = 0;
    while (env)
    {
        count++;
        env = env->next;
    }
    return count;
}

char **env_to_array(t_env *env)
{
    int count = count_env_vars(env);
    char **arr = gc_malloc(sizeof(char *) * (count + 1));
    int i = 0;
    
    while (env)
    {
        arr[i] = gc_malloc(ft_strlen(env->key) + (env->value ? ft_strlen(env->value) : 0) + 3);
        ft_strcpy(arr[i], env->key);
        if (env->value)
        {
            ft_strcat(arr[i], "=");
            ft_strcat(arr[i], env->value);
        }
        i++;
        env = env->next;
    }
    arr[i] = NULL;
    return arr;
}

void sort_env_array(char **arr, int size)
{
    int i, j;
    char *temp;
    
    for (i = 0; i < size - 1; i++)
    {
        for (j = 0; j < size - i - 1; j++)
        {
            if (ft_strcmp(arr[j], arr[j + 1]) > 0)
            {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

int is_valid_identifier(char *str)
{
    int i;
    
    if (!str || !str[0] || ft_isdigit(str[0]) || str[0] == '=')
        return (0);
    
    i = 0;
    while (str[i] && str[i] != '=')
    {
        if (!ft_isalnum(str[i]) && str[i] != '_' && 
            !(str[i] == '+' && str[i+1] == '='))
            return (0);
        i++;
    }
    return (1);
}

t_env *find_env_var(t_env *env, char *key)
{
    while (env)
    {
        if (!ft_strncmp(env->key, key, -1))
            return env;
        env = env->next;
    }
    return NULL;
}
char *handle_dollar_value(char *value)
{
    char *dollar_pos;
    char *result;
    
    if (!value)
        return NULL;

    dollar_pos = ft_strchr(value, '$');
    if (!dollar_pos || !*(dollar_pos + 1))
        return ft_strdup(value);
    if (ft_isdigit(*(dollar_pos + 1)))
    {
        // Create a new string: content before $ + content after the first digit
        int prefix_len = dollar_pos - value;
        char *suffix = dollar_pos + 2;  // Skip the $ and the first digit
        
        result = gc_malloc(prefix_len + ft_strlen(suffix) + 1); // Allocate memory for prefix, suffix and null terminator
        if (prefix_len)
            ft_strncpy(result, value, prefix_len);
        if (suffix)
            ft_strcpy(result + prefix_len, suffix); // Copy the suffix after the prefix
    }
    else
        result = ft_strdup(value);  // For now, just copy the value as is
    
    return result;
}

void update_or_add_env(t_env *env, char *arg, t_variables vars)
{
    int append = 0;
    char *key;
    char *equals_pos = ft_strchr(arg, '=');
    char *plus_pos = ft_strchr(arg, '+');
    
    if (plus_pos && equals_pos && plus_pos + 1 == equals_pos)
    {
        append = 1;
        key = ft_substr(arg, 0, plus_pos - arg);
    }
    else if (equals_pos)
    {
        key = ft_substr(arg, 0, equals_pos - arg);
    }
    else
    {
        key = ft_strdup(arg);
    }
    
    char *value = NULL;
    char *temp_value = NULL;
    char *processed_value = NULL;
    
    if (equals_pos)
    {
        temp_value = ft_substr(equals_pos + 1, 0, ft_strlen(equals_pos + 1));
        if (temp_value && temp_value[0] == '"' && 
            temp_value[ft_strlen(temp_value) - 1] == '"')
        {
            value = ft_substr(temp_value, 1, ft_strlen(temp_value) - 2);
            gc_remove_ptr(temp_value);
        }
        else
        {
            value = temp_value;
        }
        
        // Handle $ in value
        if (value)
        {
            processed_value = handle_dollar_value(value);
            gc_remove_ptr(value);
            value = processed_value;
        }
    }
    
    t_env *existing = find_env_var(env, key);
    if (existing)
    {
        if (equals_pos)  // Only update if there's a new value
        {
            if (append && existing->value)
            {
                char *new_value = ft_strjoin(existing->value, value);
                // gc_remove_ptr(existing->value);
                gc_remove_ptr(value);
                existing->value = new_value;
            }
            else
            {
                gc_remove_ptr(existing->value);
                existing->value = value;
            }
        }
    }
    else
    {
        t_env *new_node = gc_malloc(sizeof(t_env));
        new_node->key = ft_strdup(key);
        new_node->value = value;
        new_node->next = NULL;
        add_back(vars.env, new_node);
    }
    gc_remove_ptr(key);
}

void print_export_env(t_env *env)
{
    int size = count_env_vars(env);
    char **env_arr = env_to_array(env);
    
    sort_env_array(env_arr, size);
    
    for (int i = 0; env_arr[i]; i++)
    {
        char *equals = ft_strchr(env_arr[i], '=');
        ft_putstr_fd("declare -x ", 1);
        if (equals)
        {
            *equals = '\0';
            ft_putstr_fd(env_arr[i], 1);
            ft_putstr_fd("=\"", 1);
            ft_putstr_fd(equals + 1, 1);
            ft_putstr_fd("\"", 1);
        }
        else
        {
            ft_putstr_fd(env_arr[i], 1);
        }
        ft_putstr_fd("\n", 1);
        gc_remove_ptr(env_arr[i]);
    }
    gc_remove_ptr(env_arr);
}

int export_(char **args, t_env *env, t_variables vars)
{
    int i = 1;  // Start from args[1] as args[0] is "export"
    
    if (!args[1])
    {
        print_export_env(env);
        return 0;
    }
    
    while (args[i])
    {
        if (!is_valid_identifier(args[i]))
        {
            ft_putstr_fd("minishell: export: `", 2);
            ft_putstr_fd(args[i], 2);
            ft_putstr_fd("': not a valid identifier\n", 2);
        }
        else
        {
            update_or_add_env(env, args[i], vars);
        }
        i++;
    }
    
    return 0;
}
