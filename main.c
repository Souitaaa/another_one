/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csouita <csouita@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 19:53:48 by csouita           #+#    #+#             */
/*   Updated: 2024/11/19 18:25:04 by csouita          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int last_exit_status;
t_global g_global;

void	handle_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	g_global.last_exit_status =130;
}

void	handle_sigint2(int sig)
{
	(void)sig;
	// write(1, "\n", 1);
	g_global.last_exit_status =130;
}
void	write_int(int sig)
{
	(void)sig;
	write(2, "\n", 1);
	g_global.last_exit_status = 128 + SIGINT;
}

void	write_quit(int sig)
{
	(void)sig;
	write(2, "Quit (core dumped)\n", 19);
	g_global.last_exit_status = 128 + SIGQUIT;
}


int	search_in(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (i);
		i++;
	}
	return (i);
}

void	add_back(t_env **head, t_env *new_content)
{
	t_env	*last;

	if (!head || !new_content)
		return ;
	if (*head)
	{
		last = get_last_node(*head);
		last->next = new_content;
	}
	else
		*head = new_content;
}

t_env	*cp_env_in_liste(char **env)
{
	int		i;
	t_env	*lst;

	i = 0;
	lst = 0;
	while (env[i])
	{
		add_back(&lst, create_node(env[i]));
		i++;
	}
	return (lst);
}
void free_env_list(t_env *env)
{
    t_env *temp;
    while (env) {
        temp = env;
        env = env->next;
        gc_remove_ptr(temp->key);
        gc_remove_ptr(temp->value);
        gc_remove_ptr(temp);
    }
}
void free_file_list(t_file *file)
{
    t_file *temp;
    while (file) {
        temp = file;
        file = file->next;
        gc_remove_ptr(temp->file_name);  
        gc_remove_ptr(temp);              
    }
}
void free_data(t_data *data)
{
    while (data) {
        gc_remove_ptr(data->line);           
        free_lexer_list(data->head);
        t_data *temp = data;
        data = data->next;
        gc_remove_ptr(temp);                  
    }
}
void free_lexer_list(t_lexer *head) 
{
    t_lexer *temp;
    while (head) {
        temp = head;
        head = head->next;
        gc_remove_ptr(temp->str); 
        gc_remove_ptr(temp);       
    }
}

void free_command_list(t_command *cmd)
{
    t_command *temp;
    while (cmd) {
        temp = cmd;

        if (temp->cmd) {
            for (int i = 0; temp->cmd[i]; i++) {
                gc_remove_ptr(temp->cmd[i]);
            }
            gc_remove_ptr(temp->cmd); 
        }
        free_file_list(temp->file);
        cmd = cmd->next;
        gc_remove_ptr(temp);
    }
}

void free_resources(t_data *data, t_env *env, t_command *cmd)
{
    free_env_list(env);
    free_command_list(cmd);
    free_lexer_list(data->head);
    gc_remove_ptr(data->line);
}

int	initialize_vars(t_variables *vars)
{
	vars->result = NULL;
	vars->pid_str = NULL;
	vars->count_str = NULL;
	vars->total_len = 0;
	vars->heredoc_count = 0;
	vars->last_fd = -1;
	vars->pipe_pids = NULL;
	vars->pipe_fds = NULL;
	vars->pipe_count = 0;
	return (0);
}

int	main(int ac, char *av[], char **envr)
{
	t_data		data;
	t_env		*env;
	t_variables	vars;
	t_command	*cmd;
	int			heredoc_fd;

	(void)av;
	vars.env = &env;
	if (ac != 1)
	{
		write(2, "Error in arguments", 19);
		exit(1);
	}
	signal(SIGINT, handle_sigint);
	env = cp_env_in_liste(envr);
	initialize_vars(&vars);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		handle_signals(before_readline);
		data.line = readline("minishell> ");
		handle_signals(in_parent);
		if (!data.line)
		{
			printf("exit\n");
			break ;
		}
		if (ft_strlen(data.line) > 0)
			add_history(data.line);
		lexer(&data);
		if (syntax_error(&data) == 0)
		{
			ft_putstr_fd("syntax error\n", 2);
			continue ;
		}
		expand(data.head, env);
		cmd = parser(&data);
		heredoc_fd = -1;
		if (count_heredocs(cmd) > 0)
		{
			heredoc_fd = multi_heredoc(cmd, env);
			if (heredoc_fd == -1)
			{
				free_command_list(cmd);
				gc_remove_ptr(data.line);
				continue ;
			}
		}
		if (exec_cmd(cmd, env, heredoc_fd, &vars) == -1)
		{
			perror("execution");
			free_command_list(cmd);
			gc_remove_ptr(data.line);
			exit(127);
		}
		free_command_list(cmd);
		gc_remove_ptr(data.line);
	}
	free_env_list(env);
	gc_free_all();
	return (0);
}
