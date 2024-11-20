/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csouita <csouita@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 20:19:43 by mboumlik          #+#    #+#             */
/*   Updated: 2024/11/19 18:25:12 by csouita          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./minishell.h"

void cleanup_heredoc_info(t_heredoc_info **heredocs, int count)
{
    int i;

    if (!heredocs || count <= 0)
        return;
        
    for (i = 0; i < count; i++)
    {
        if (heredocs[i])
        {
            if (heredocs[i]->filename)
            {
                unlink(heredocs[i]->filename);
                gc_remove_ptr(heredocs[i]->filename);
                heredocs[i]->filename = NULL;
            }
            if (heredocs[i]->delimiter)
            {
                gc_remove_ptr(heredocs[i]->delimiter);
                heredocs[i]->delimiter = NULL;
            }
            gc_remove_ptr(heredocs[i]);
            heredocs[i] = NULL;
        }
    }
    gc_remove_ptr(heredocs);
}

int count_name_file(t_variables *var)
{
    return (ft_strlen("/tmp/heredoc_") + ft_strlen(var->pid_str) +
            ft_strlen("_") + ft_strlen(var->count_str) + 1);
}

char *allocate_name_file(t_variables *var)
{
    var->total_len = count_name_file(var);
    var->result = (char *)gc_malloc(var->total_len);
    if (!var->result)
    {
        gc_remove_ptr(var->pid_str);
        gc_remove_ptr(var->count_str);
    }
    return (var->result);
}

void join_name_file(t_variables *var)
{
    ft_strlcpy(var->result, "/tmp/heredoc_", var->total_len);
    ft_strlcat(var->result, var->pid_str, var->total_len);
    ft_strlcat(var->result, "_", var->total_len);
    ft_strlcat(var->result, var->count_str, var->total_len);
}

char *generate_temp_filename(void)
{
    static int counter = 0;
    t_variables var;

    var.pid_str = ft_itoa(1111);
    if (!var.pid_str)
        return (NULL);
    var.count_str = ft_itoa(counter++);
    if (!var.count_str)
    {
        gc_remove_ptr(var.pid_str);
        return (NULL);
    }
    if (!allocate_name_file(&var))
        return (NULL);
    join_name_file(&var);
    gc_remove_ptr(var.pid_str);
    gc_remove_ptr(var.count_str);
    return (var.result);
}


int count_heredocs(t_command *cmd)
{
    t_file *current;
    int count;
    count = 0;
    if (!cmd || !cmd->file)
    {
      return 0;  
    }
    
    current = cmd->file;
    while (current) {
        if (current->file_type == HEREDOC)
            count++;
        current = current->next;
    }
    return count;
}


void free_herdoc(t_heredoc_info **heredocs,int j)
{
    gc_remove_ptr(heredocs[j]->filename);
    gc_remove_ptr(heredocs[j]->delimiter);
    gc_remove_ptr(heredocs[j]);
}
int handle_error_cleanup(t_heredoc_info **heredocs, int count)
{
    int j;
    j =0;
    while(j < count)
    {
        free_herdoc(heredocs, j);
        j++;
    }
    gc_remove_ptr(heredocs);
    return 0;
}
t_heredoc_info *init_single_heredoc(t_file *file)
{
    t_heredoc_info *heredoc = gc_malloc(sizeof(t_heredoc_info));
    if (!heredoc)
        return NULL;

    heredoc->filename = generate_temp_filename();
    if (!heredoc->filename)
    {
        gc_remove_ptr(heredoc);
        return NULL;
    }

    heredoc->delimiter = ft_strdup(file->file_name);
    if (!heredoc->delimiter)
    {
        gc_remove_ptr(heredoc->filename);
        gc_remove_ptr(heredoc);
        return NULL;
    }
    return heredoc;
}
t_heredoc_info **init_heredoc_info(t_command *command, int heredoc_count)
{
    t_heredoc_info **heredocs;
    t_file *current;
    int i;

    if (!command || heredoc_count <= 0)
        return NULL;

    heredocs = gc_malloc(heredoc_count * sizeof(t_heredoc_info *));
    if (!heredocs)
        return NULL;
    current = command->file;
    i = 0;
    while (current && i < heredoc_count)
    {
        if (current->file_type == HEREDOC)
        {
            heredocs[i] = init_single_heredoc(current);
            if (!heredocs[i])
                return (handle_error_cleanup(heredocs, i), NULL);
            i++;
        }
        current = current->next;
    }
    return heredocs;
}

void write_to_heredoc(int fd, char *delimiter, t_env *env, int is_quoted)
{
    char *line;
    
    
    while (1)
    {
        handle_signals(in_heredoc);   
        line = readline("> ");
        if (!line) 
        {
            write(2, "\n", 1);
            break;
        }
        if (ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1) == 0)
        {
            gc_remove_ptr(line);
            break;
        }
        if (!is_quoted)
            expand_in_heredoc(&line, env);
        ft_putendl_fd(line, fd);
        gc_remove_ptr(line);
        line = NULL;  
    }
}
//////////////////////////////////////////////////

t_file *find_current_heredoc(t_command *command, int index)
{
    t_file *current;
    int count;

    current = command->file;
    count = 0;
    while (current)
    {
        if (current->file_type == HEREDOC)
        {
            if (count == index)
                return (current);
            count++;
        }
        current = current->next;
    }
    return (NULL);
}
int handle_child_process_herdoc(t_heredoc_info *heredoc, t_env *env, t_file *current_file)
{
    int fd;

    fd = open(heredoc->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
        exit(1);
    write_to_heredoc(fd, heredoc->delimiter, env, current_file->is_qouted);
    close(fd);
    exit(0);
}

int handle_parent_process(pid_t pid, t_heredoc_info **heredocs, int heredoc_count)
{
    int status;

    if (waitpid(pid, &status, 0) == -1)
    {
        cleanup_heredoc_info(heredocs, heredoc_count);
        return (0);
    }

    if (WTERMSIG(status))
    {
        write(1, "\n", 1);
        g_global.last_exit_status = 130;
    }
    
    if (WIFSIGNALED(status) || WEXITSTATUS(status) != 0)
    {
        cleanup_heredoc_info(heredocs, heredoc_count);
        return (0);
    }
    return (1);
}

int process_heredoc(t_heredoc_info **heredocs, int i, 
    t_command *command, t_env *env)
{
    pid_t pid;
    t_file *current_file;

    current_file = find_current_heredoc(command, i);
    if (!current_file)
        return (-1);
    
    pid = fork();
    if (pid == -1)
    {
        cleanup_heredoc_info(heredocs, i);
        return (-1);
    }
    if (pid == 0)
        handle_child_process_herdoc(heredocs[i], env, current_file);
    if (!handle_parent_process(pid, heredocs, i))
        return (-1);
    return (1);
}

int clean_herdoc(int fd, t_heredoc_info **heredocs, int count)
{
    if (fd == -1)
    {
        cleanup_heredoc_info(heredocs, count);
        return (-1);
    }
    cleanup_heredoc_info(heredocs, count);
    return fd;
}

int multi_heredoc(t_command *command, t_env *env)
{
    t_heredoc_info **heredocs;
    t_variables var;
    int i;
    
    i = -1;
    var.heredoc_count = count_heredocs(command);
    
    if (var.heredoc_count <= 0)
        return (-1);
    if (var.heredoc_count > 16)
    {
        printf("minishell: maximum here-document count exceeded\n");
        return (-1);  // Changed from exit(2) to return error
    }
    heredocs = init_heredoc_info(command, var.heredoc_count);
    if (!heredocs)
        return (-1);
    while (++i < var.heredoc_count)
    {
        if (process_heredoc(heredocs, i, command, env) == -1)
        {
            cleanup_heredoc_info(heredocs, var.heredoc_count);
            return (-1);
        }
        if (i > 0 && i < var.heredoc_count - 1)
            unlink(heredocs[i - 1]->filename);
    }
    var.last_fd = open(heredocs[var.heredoc_count - 1]->filename, O_RDONLY);
    return clean_herdoc(var.last_fd, heredocs, var.heredoc_count);
}

