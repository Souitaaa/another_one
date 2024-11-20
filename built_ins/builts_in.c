#include "../minishell.h"


int exec_built_in(char **arr,t_env *env, t_variables vars)
{
    int status = 0;
    
    if (!arr || !arr[0])
        return (0);
    if (!ft_strncmp(arr[0], "pwd", -1))
        status = pwd_(env);
    else if (!ft_strncmp(arr[0], "env", -1))
        status = env_(arr, env);
    else if (!ft_strncmp(arr[0], "echo", -1))
        status = echo_(arr);
    else if (!ft_strncmp(arr[0], "exit", -1))
        status = exit_(&arr[1]);
    else if (!ft_strncmp(arr[0], "cd", -1))
        status = cd_(arr, env);
    else if (!ft_strncmp(arr[0], "export", -1))
        status = export_(arr, env, vars);
    else if (!ft_strncmp(arr[0], "unset", -1))
        status = unset_(arr, &env, vars);
    else
    {
        ft_putstr_fd("minishell: command not found: ", 2);
        ft_putstr_fd(arr[0], 2);
        ft_putstr_fd("\n", 2);
        gc_free_all();
        return (127);
    }
    return (status);
}
