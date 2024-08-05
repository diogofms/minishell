/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abekri <abekri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 22:46:27 by abekri            #+#    #+#             */
/*   Updated: 2024/08/04 04:08:35 by abekri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	print_error(char *s, int *exit_cd)
{
	if (s[0] == '\"' && s[1] == '\"')
	{
		printf("minishell: : command not found\n");
		*exit_cd = CDM_NOT_EXIST;
	}
	else if (ft_strncmp(s, "&&", 3) == 0)
	{
		printf("minishell: syntax error near unexpected token `%s'\n", s);
		*exit_cd = ERR_SYNTX_COD;
	}
}

int	check_parsing(char *input, char **envp, int *fds, int *exit_cd)
{
	char	**cmds;
	int		token_count;

	if (!perform_lexical_analysis(input, &cmds, fds, exit_cd))
		return (0);
	token_count = 0;
	while (cmds[token_count] != NULL)
		token_count++;
	if (!check_syntax(fds, cmds, token_count, exit_cd))
		return (0);
	if (!generate_abstract_cmds(cmds, token_count, envp, exit_cd))
		return (0);
	if (!expansion_success(envp, input, fds, exit_cd))
		return (0);
	if (!handle_redirections(cmds, token_count, fds, exit_cd))
		return (0);
	return (1);
}

void	handle_error(char *input, int *exit_cd)
{
	print_error(input, exit_cd);
}

void	initialize_command_list(char command_list[MAX_COMMANDS][MAX_ARGS][BUFFER_SIZE])
{
	int	i;
	int	j;

	i = 0;
	while (i < MAX_COMMANDS)
	{
		j = 0;
		while (j < MAX_ARGS)
		{
			command_list[i][j][0] = '\0';
			j++;
		}
		i++;
	}
}

void	setup_command_context(t_command_context *context)
{
	context->input_line = NULL;
	context->command_count = 0;
	context->input_fd = 0;
	context->output_fd = 1;
	initialize_command_list(context->command_list);
}

void	finalize(char *pwd, char *home)
{
	free(pwd);
	free(home);
}

int	read_input(t_shell_config *config, t_command_context *context)
{
	char	*readline_input;

	readline_input = readline(config->terminal_name);
	if (!readline_input)
	{
		return (0);
	}
	if (readline_input[0] == '\0')
	{
		free(readline_input);
		return (1);
	}
	add_history(readline_input);
	context->input_line = readline_input;
	return (2);
}

void	process_input(t_shell_config *config, t_command_context *context)
{
	if (check_parsing(context->input_line, config->envp, config->fds,
			config->exit_cd) && context->command_count > 0
		&& context->command_list[0][0][0])
	{
		execute_command(context->command_list, context->command_count,
			&context->input_fd, &context->output_fd, config->exit_cd);
		// This function will be defined by my partner
	}
	else
	{
		handle_error(context->input_line, config->exit_cd);
	}
}

void	start_iterm(t_shell_config *config)
{
	t_command_context	context;
	int					input_status;

	setup_command_context(&context);
	while (1)
	{
		input_status = read_input(config, &context);
		if (input_status == 0)
			break ;
		if (input_status == 1)
			continue ;
		process_input(config, &context);
		free(context.input_line);
	}
	finalize(config->pwd, config->home);
}
