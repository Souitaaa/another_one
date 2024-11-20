/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csouita <csouita@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 15:51:06 by mboumlik          #+#    #+#             */
/*   Updated: 2024/11/19 18:30:24 by csouita          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./minishell.h"

void execute_this_command(char *cmd_path,char **arr,t_env *env)
{
    if (access(cmd_path, X_OK) == 0)
    {
        char **env_array = env_list_to_array(env);
        if (env_array)
        {
            // printf("cmd = [%s] \n,arr[0] =  [%s]\n,env_arr [0]%s \n",cmd_path,arr[0],env_array[0]);
            execve(cmd_path, arr, env_array);
            free_env_array(env_array);
        }
    }
    else
    {
        printf("minishell : %s : permission denied\n", arr[0]);
        gc_remove_ptr(cmd_path);
        exit(126);
    }
}

void child(char **arr,t_env *env)
{
    if (!arr || !arr[0]) {
        exit(0); 
    }
    char *cmd_path = find_command(arr[0], env);
    if (cmd_path)
    {
        if (access(cmd_path, F_OK) == 0)
        {
            execute_this_command(cmd_path,arr,env);
        }
        gc_remove_ptr(cmd_path);
        exit(1);
    }
    else
    {
        printf("%s: command not found\n", arr[0]);
        gc_free_all();
        exit(127);
    }
}

int execute_the_command(char **arr,t_env *env)
{
    int ret ;
    
    ret = 0;
    pid_t pid = fork();
    if (pid == 0)
    {
        handle_signals(in_child);
        child(arr,env);
    }
    else
    {
        int status;
        waitpid(pid, &status, 0);
        if (WIFSIGNALED(status))
        {
            if (WTERMSIG(status) == SIGQUIT)
                write_quit(status);
            else if (WTERMSIG(status) == SIGINT)
                write_int(status);
        }
        else if (WIFEXITED(status))
            g_global.last_exit_status = WEXITSTATUS(status);
        ret = g_global.last_exit_status;
    }
    return ret;
}

void close_dup(int original_stdin,int original_stdout)
{
    dup2(original_stdin, STDIN_FILENO);
    dup2(original_stdout, STDOUT_FILENO);
    close(original_stdin);
    close(original_stdout);
}
int check_built_ins(char **arr)
{
    int i;
    int is_builtin ;
    char *builtins[] = {"pwd", "env", "echo", "exit", "cd", "export", "unset", NULL};
    
    if (!arr || !arr[0])
        return 0;
    
    is_builtin = 0;
    i = 0;
    while(builtins[i])
    {
        if (ft_strncmp(arr[0], builtins[i], -1) == 0)
        {
            is_builtin = 1;
            break;
        }
        i++;
    }
    return is_builtin;
}

int execute_single_command(t_command *command, t_env *env, t_variables vars)
{
    handle_signals(in_parent);
    char **arr = command->cmd;
    int original_stdin = dup(STDIN_FILENO);
    int original_stdout = dup(STDOUT_FILENO);
    int is_builtin = 0;
    int ret = 0;
    if (command->file)
    {
        ret = handle_redirections(command);
        if (ret != 0)
        {
            close_dup(original_stdin,original_stdout);
            return ret;
        }
    }
    if(check_built_ins(arr) == 1)
        is_builtin =1;
    if (is_builtin)
        ret = exec_built_in(arr, env, vars);
    else
        ret = execute_the_command(arr,env);
    close_dup(original_stdin,original_stdout);
    return ret;
}
