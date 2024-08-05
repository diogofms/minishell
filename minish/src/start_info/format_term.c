/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   format_term.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abekri <abekri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 20:09:28 by abekri            #+#    #+#             */
/*   Updated: 2024/08/04 04:39:14 by abekri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	params_start(char **args, char *s)
{
	int	i;

	i = 0;
	while (i < 1)
	{
		args[i] = ft_strdup(s);
		i++;
	}
	args[i] = NULL;
}

void	get_args_empty(char **args)
{
	int	i;

	if (args)
	{
		i = 0;
		while (args[i])
		{
			free_str(args[i]);
			i++;
		}
		free_2d_array(args);
	}
}

void	perform_child_process_and_store_output(char **args, char *output)
{
	int		pipefd[2];
	pid_t	pid;

	if (pipe(pipefd) == -1)
		return (perror("Pipe error"));
	pid = fork();
	if (pid == 0)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		if (execve(args[0], args, NULL) == -1)
			return (perror("execve error"));
	}
	else
	{
		wait(NULL);
		close(pipefd[1]);
		read(pipefd[0], output, BUFFER_SIZE);
		close(pipefd[0]);
		dup2(1, STDOUT_FILENO);
	}
}

char	*format_prompt(char *user, char *node, char *path)
{
	char	*current_dir;
	char	*node_short;
	char	*dir_name;
	char	**parts;
	char	*output;

	parts = ft_split(node, '.');
	node_short = ft_strdup(parts[0]);
	free_2d_array(parts);
	current_dir = ft_strdup(ft_strrchr(path, '/') + 1);
	if (ft_strchr(current_dir, '\n'))
		dir_name = ft_substr(current_dir, 0, ft_strlen(current_dir) - 1);
	else
		dir_name = ft_strdup(current_dir);
	free_str(current_dir);
	output = ft_strdup(node_short);
	free_str(node_short);
	output = ft_strjoin(output, ":");
	output = ft_strjoin(output, dir_name);
	free_str(dir_name);
	output = ft_strjoin(output, " ");
	output = ft_strjoin(output, user);
	free_str(user);
	output = ft_strjoin(output, "$ ");
	return (output);
}

char	*format_iterm(char **environment)
{
	char	*param_cmd[2];
	char	*user;
	char	*current_dir;
	char	*node;
	char	output[BUFFER_SIZE];

	params_start(param_cmd, "/bin/hostname");
	perform_child_process_and_store_output(param_cmd, output);
	node = ft_strdup(output);
	get_args_empty(param_cmd);
	params_start(param_cmd, "/bin/pwd");
	perform_child_process_and_store_output(param_cmd, output);
	current_dir = ft_strdup(output);
	get_args_empty(param_cmd);
	user = ft_strdup(environment[find_var_index(environment, "USER")]);
	return (format_prompt(user, node, current_dir));
}
