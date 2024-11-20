/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csouita <csouita@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 15:16:32 by mboumlik          #+#    #+#             */
/*   Updated: 2024/11/19 18:30:18 by csouita          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int count_commands(t_command *command)
{
    t_command *tmp;
    int cmd_count = 0;
    
    tmp = command;
    while (tmp)
    {
        cmd_count++;
        tmp=tmp->next;
    }
    return cmd_count;
}

void	free_process_and_pipes(t_variables *vars)
{
	if (vars->pipe_pids)
		gc_remove_ptr(vars->pipe_pids);
	if (vars->pipe_fds)
		gc_remove_ptr(vars->pipe_fds);
	vars->pipe_pids = NULL;
	vars->pipe_fds = NULL;
}
void	close_all_pipes(t_variables *vars)
{
	int	i;

	i = 0;
	while (i < vars->pipe_count)
	{
		close(vars->pipe_fds[i][0]);
		close(vars->pipe_fds[i][1]);
		i++;
	}
}
void close_pip(t_variables *vars, int i )
{
	while (i >= 0)
	{
		close(vars->pipe_fds[i][0]);
		close(vars->pipe_fds[i][1]);
		i--;
	}
	
}
int	setup_pipes(t_variables *vars)
{
	int	i;

	i = 0;
	while (i < vars->pipe_count)
	{
		if (pipe(vars->pipe_fds[i]) == -1)
		{
			close_pip(vars , --i);
			perror("pipe");
			return (0);
		}
		i++;
	}
	return (1);
}



void	setup_child_pipes(int cmd_index, int cmd_count, t_variables *vars)
{
	if (cmd_index > 0)
		dup2(vars->pipe_fds[cmd_index - 1][0], STDIN_FILENO);
	if (cmd_index < cmd_count - 1)
		dup2(vars->pipe_fds[cmd_index][1], STDOUT_FILENO);
}

int handle_child_process(t_command *cmd, int cmd_index, int cmd_count, t_variables *vars, t_env *env)
{
    char *builtins[] = {"pwd", "env", "echo", "exit", "cd", "export", "unset", NULL};
    int is_builtin;
	
	is_builtin = 0;
    if (cmd->cmd && cmd->cmd[0])
    {
        int i = 0;
        while (builtins[i])
        {
            if (ft_strncmp(cmd->cmd[0], builtins[i], -1) == 0)
            {
                is_builtin = 1;
                break;
            }
            i++;
        }
        if (!is_builtin)
        {
            char *cmd_path = find_command(cmd->cmd[0], env);
            if (!cmd_path)
            {
                printf("%s : command not found\n", cmd->cmd[0]);
				gc_free_all();
                return (0);
            }
            gc_remove_ptr(cmd_path);
        }
    }
    setup_child_pipes(cmd_index, cmd_count, vars);
    close_all_pipes(vars);
    free_process_and_pipes(vars);
    execute_single_command(cmd, env, *vars);
    exit(1);
}

void	wait_for_children(t_variables *vars, int cmd_count)
{
	int	i;
	int	status;

	i = 0;
	while (i < cmd_count)
	{
		waitpid(vars->pipe_pids[i], &status, 0);
		if (i == cmd_count - 1)
		{
			if (WIFEXITED(status))
				g_global.last_exit_status = WEXITSTATUS(status);
		}
		i++;
	}
}

int	execute_child(t_command *current, int cmd_index,int cmd_count, t_variables *vars, t_env *env)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return (0);
	if (pid == 0 && !handle_child_process(current, cmd_index,
			cmd_count, vars, env))
		exit(127);
	if (pid > 0)
		vars->pipe_pids[cmd_index] = pid;
	return (1);
}

void	execute_multi_commands(t_command *command, t_env *env,t_variables *vars)
{
	t_command	*current;
	int			cmd_index;
	int			cmd_count;

	cmd_count = count_commands(command);
	vars->pipe_count = cmd_count - 1;
	vars->pipe_pids = gc_malloc(sizeof(pid_t) * cmd_count);
	vars->pipe_fds = gc_malloc(sizeof(int[2]) * vars->pipe_count);
	if (!vars->pipe_pids || !vars->pipe_fds || !setup_pipes(vars))
	{
		free_process_and_pipes(vars);
		return ;
	}
	current = command;
	cmd_index = 0;
	while (cmd_index < cmd_count)
	{
		if (!execute_child(current, cmd_index, cmd_count, vars, env))
			break ;
		current = current->next;
		cmd_index++;
	}
	close_all_pipes(vars);
	wait_for_children(vars, cmd_count);
	free_process_and_pipes(vars);
}

void	execute_piped_commands(t_command *command, t_env *env,t_variables *vars)
{
	int	cmd_count;
	cmd_count = count_commands(command);
	if (cmd_count == 1)
	{
		execute_single_command(command, env, *vars);
		return ;
	}
	else
		execute_multi_commands(command, env, vars);
}


int	exec_cmd(t_command *command, t_env *env, int heredoc_fd, t_variables *vars)
{
	int	original_stdin;

	if (!command)
		return (1);
	original_stdin = -1;
	if (heredoc_fd != -1)
	{
		original_stdin = dup(STDIN_FILENO);
		dup2(heredoc_fd, STDIN_FILENO);
		close(heredoc_fd);
	}
	execute_piped_commands(command, env, vars);
	if (original_stdin != -1)
	{
		dup2(original_stdin, STDIN_FILENO);
		close(original_stdin);
	}
	return (0);
}
