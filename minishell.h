/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboumlik <mboumlik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 19:54:21 by csouita           #+#    #+#             */
/*   Updated: 2024/11/19 12:01:41 by mboumlik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <readline/readline.h>
#include <unistd.h>
#include <readline/history.h>
#include <signal.h>
#include <stdbool.h>
#include <linux/limits.h>
#include <errno.h>
#include <sys/wait.h>
#include <fcntl.h>

#define in_heredoc 500
#define in_parent 501
#define in_child 502
#define before_readline 503


typedef struct s_gc_node
{
    void *ptr;              
    int is_freed;           
    struct s_gc_node *next; 
} t_gc_node;

typedef struct 		s_global
{
    t_gc_node *gc_list;
	int last_exit_status;
} 					t_global;

extern t_global g_global;

typedef struct 		s_temp_files
{
    char            *filename;    
    struct s_temp_files    *next; 
} 					t_temp_files;

typedef enum 			e_num
{
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	HEREDOC,
	APPEND,
	WHITESPACE,
	QUOTES
}						t_tokens;


typedef struct s_env
{
	char				*value;
	char				*key;
	struct s_env		*next;
}						t_env;

typedef struct s_lexer
{
	char				*str;
	t_tokens			tokens;
	struct s_lexer		*next;
}						t_lexer;

typedef struct s_data
{
	char				*line;
	struct s_data		*next;
	t_lexer				*head;
}						t_data;

typedef struct list
{
	int					content;
	struct list			*next;
}						t_list;

typedef struct s_file
{
	char				*file_name;
	t_tokens			file_type;
	int					is_qouted;
	int					is_ambigous;
	struct s_file		*next;
}						t_file;

typedef struct s_command
{
	char				**cmd;
	t_file				*file;
	struct s_command	*next;
}						t_command;

typedef struct s_heredoc_info {
    char *filename;
    char *delimiter;
} t_heredoc_info;

typedef struct variables
{
	char        *result;
   char        *pid_str;
   char        *count_str;
   int         total_len;
   int heredoc_count;
    int last_fd;
	
	pid_t		*pipe_pids;
	int			(*pipe_fds)[2];
	int			pipe_count;
	t_env		**env;
}t_variables;



t_env *get_node_by_key_from_env(t_env *env, char *key);

char					*ft_strchr(char *s, int c);
int						ft_strchr_(char *s, int c);
void					*ft_memcpy(void *dest,void *src, size_t n);
int						ft_isalnum(int c);
char					*ft_strjoin(char *s1, char *s2);
int						ft_isdigit(int c);
int						ft_strcmp(char *s1, char *s2);
size_t					ft_strlen(char *str);
void					ft_putstr_fd(char *s, int fd);
void					add_spaces(t_data *data);
char					**ft_split(char *s, char c);
t_list					*ft_lstlast(t_list *lst);
int						ft_lstsize(t_list *lst);
t_list					*ft_lstnew(int content);
void					lexer(t_data *line);
void					ft_lstadd_back_lexer(t_lexer **lst, t_lexer *new);
void					ft_lstadd_back_file(t_file **lst, t_file *new);
void					ft_lstadd_back_command(t_command **lst, t_command *new);
t_lexer					*new_lexer_node(t_tokens type, char *str);
void					add_node(t_lexer **head, t_tokens type, char *str);
char					*ft_strdup(char *s1);
char					*ft_substr(char *s, unsigned int start, size_t len);
// char					*add_spaces2(t_data *data);
int						syntax_error(t_data *data);
void					expand(t_lexer *lexer, t_env *env);
void					the_expandables(t_lexer **lexer, t_env *env,
							char **str_to_expand);
int						check_quotes_in_expand(char *str);
char					*get_key(char *str);
char					*get_value(t_env *env, char *key);
void					expand_in_heredoc(char **str, t_env *env);
void					not_expandables(t_lexer **lexer);
int						cheking_the_expand(char *str, t_env *env, int *i,
							char **str_to_expand);
char					*add_escape_characters(char *str);
int						after_quote(t_lexer **lexer, char **str_to_expand);
void					special_case(t_lexer *lexer, char **str_to_expand,
							int *i);
char					**get_key_before_expand(void);
void					special_case_in_heredoc(char *str, char **str_to_expand,
							int *i);
void					dollar_and_quote(char **str_to_expand, int *i);
t_command				*parser(t_data *data);
void					parser_works(char **command, t_lexer **head,
							t_file **file_name);
int						is_redirection(t_tokens token);
void					handle_redirection(t_lexer **head, t_file **file_name,
							int redirection_type);
void					handle_word(char **command, t_lexer **head);
char					*handle_quotes(char *str);
int						check_redirection(char *line);
char					**ft_split00(char *s);
int						has_quote(char *delim);
int						ft_is_void(char c);
int						check_quotes_in_expand(char *str);
int						is_whites_spaces(char c);
int						check_pipe(char *s);
int						check_quotes(t_data *data);
t_env					*create_node(char *env);
t_env					*get_last_node(t_env *head);
t_file					*ft_create_node(char *file_name, t_tokens redirection_type,
		int is_ambigous, int is_qouted);
t_command				*ft_add_command(char *command, t_file *file);
void					ft_create_command(t_command **command_list, char *command,
							t_file **file);
void					handle_redirection(t_lexer **head, t_file **file_name,
		int redirection_type);
int						count_arg(char **str);
// int						check_qoutes(char *str);
int						search_in(char *str, char c);


void					display_token_command(t_command *command);
void					display_token_lexer(t_lexer *lexer);
char					*get_token(t_tokens token);
void					handle_sigint(int sig);
char					*remove_escape_characters(char *str);
char					*add_escape_characters(char *str);
int						check_qoutes2(char *str);
void 					*gc_malloc(size_t size);
void					gc_free_all(void);
void					gc_remove_ptr(void *ptr);
char					*gc_strdup(char *s);






///////////////////execution/////////////////

int execute_single_command(t_command *command,t_env *env,t_variables vars);
// int exec_cmd(t_command *command,t_env *env,int herdoc_fd);
int	exec_cmd(t_command *command, t_env *env, int heredoc_fd, t_variables *vars);
int exec_built_in(char **arr,t_env *env, t_variables vars);
int pwd_(t_env *env);
int echo_(char **args);
char *get_env_value(t_env *env, const char *key);
int env_(char **args,t_env *env);
t_env *cp_env_in_liste(char **env);
void add_back(t_env **head ,t_env *new_content);
t_env *get_last_node(t_env *head);
t_env *create_node(char *env);
int export_(char **args, t_env *env, t_variables vars);
void print_export_env(t_env *env);
void update_or_add_env(t_env *env, char *arg, t_variables vars);
char *handle_dollar_value(char *value);
t_env *find_env_var(t_env *env, char *key);
int is_valid_identifier(char *str);
void sort_env_array(char **arr, int size);
char **env_to_array(t_env *env);
int count_env_vars(t_env *env);
int is_valid_env_name(char *str);
void remove_env_var(t_env **env, char *key, t_variables vars);
int unset_(char **args, t_env **env, t_variables vars);
void update_env_var(t_env *env, char *key, char *value);
int cd_(char **args, t_env *env);
int exit_(char **args);
char *find_command(char *cmd,t_env *env);
char **env_list_to_array(t_env *env);
void	ft_putendl_fd(char *s, int fd);
int multi_heredoc(t_command *command, t_env *env);
int count_heredocs(t_command *cmd);
void	*ft_memset(void *big, int to_find, size_t len);

/////////////////////utils_execution////////////
int ft_strncmp(const char *s1, const char *s2, size_t n);
void	ft_putchar_fd(char c, int fd);
char	*ft_itoa(int n);
char	*ft_strcpy(char *dest, char *src);
char	*ft_strcat(char *dest, char *src);
char    *ft_strncpy(char *dest,  char *src, unsigned int n);
size_t	ft_strlcat(char *dest, char *src, size_t n);
size_t	ft_strlcpy(char *dst, const char *src, size_t n);
long ft_atoi(const char *str);
int handle_redirections(t_command *command);
void	execute_piped_commands(t_command *command, t_env *env,t_variables *vars);
void	*ft_calloc(size_t count, size_t size);
void	handle_sigint2(int sig);


///////////////////////free/////////////////////
void free_command_list(t_command *cmd);
void free_lexer_list(t_lexer *head);
void free_data(t_data *data);
void free_file_list(t_file *file);
void free_env_list(t_env *env);
void free_env_array(char **env_array);
void handle_signals(int sig);
// void	handle_sigint2(int sig);
void	write_int(int sig);
void	write_quit(int sig);

#endif
