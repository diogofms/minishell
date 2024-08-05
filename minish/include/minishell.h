/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abekri <abekri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 22:46:11 by abekri            #+#    #+#             */
/*   Updated: 2024/08/04 04:12:54 by abekri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
# define MAX_TOKENS 1000
# define WORD 0
# define R_DIR_APPEND 1
# define H_DOC 2
# define ERR 3
# define EOL 4
# define ERR_SYNTX_MSG "Syntax error near token: %c\n"
# define ERR_SYNTX_MSG_RE_H "Syntax error: expected %s before end of line\n"
# define MAX_TOKENS 100
# define MAX_COMMANDS 10
# define MAX_ARGS 50
# define MAX_ENV 50
# define BUFFER_SIZE 256
# define DEBUG 0
# define TERMINAL_NAME "minishell"
# define MAX_ENV_VARS 1024
# define MAX_TOKENS 512
# define MAX_COMMANDS 512
# define MAX_ARGS 128
# define MAX_STRING_LEN 256

extern volatile sig_atomic_t	g_signal;

typedef enum e_error_cod
{
	CMD_NOT_EXEC = 126,
	ERR_SYNTX_COD = 258,
	CTRL_C = 130,
	CDM_NOT_EXIST = 127,
}								t_error_cod;

typedef struct s_shell_config
{
	char						*terminal_name;
	char						*pwd;
	char						*home;
	int							*exit_cd;
	char						**envp;
	int							*fds;
}								t_shell_config;

typedef struct s_command_context
{
	char						*input_line;
	char						command_list[MAX_COMMANDS][MAX_ARGS][BUFFER_SIZE];
	int							command_count;
	int							input_fd;
	int							output_fd;
}								t_command_context;

void							init_shell(void);
void							setup_signal_handlers(void);
void							start_iterm(t_shell_config *config);
int								check_parsing(char *input, char **envp,
									int *fds, int *exit_cd);
int								check_syntax(int *token_types,
									char **token_values, int token_count,
									int *exit_cd);
int								perform_lexical_analysis(char *readline,
									char ***cmds, int *fds, int *exit_cd);
int								generate_abstract_cmds(char **cmds,
									int token_count, char **envp, int *exit_cd);
int								expansion_success(char **envp, char *input,
									int *fds, int *exit_cd);
int								handle_redirections(char **command_args,
									int command_count, int *file_descriptors,
									int *exit_code);
void							print_2d_array(char **arr, char *msg);
void							adjust_shell_lvl(char **env, int increment);
char							*format_prompt(char *environment[]);
char							*format_iterm(char **environment);

#endif
