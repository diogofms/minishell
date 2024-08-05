/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abekri <abekri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/27 05:37:35 by abekri            #+#    #+#             */
/*   Updated: 2024/07/29 04:20:01 by abekri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	remove_elements(char **args_array, int *current_position,
		int num_elements_to_remove)
{
	int	total_args;
	int	start_index;
	int	end_index;

	total_args = -1;
	start_index = *current_position - 1;
	end_index = *current_position;
	while (args_array[++total_args])
		;
	while (end_index < *current_position + num_elements_to_remove)
		free_str(args_array[end_index++]);
	while (++start_index < total_args - num_elements_to_remove)
		args_array[start_index] = args_array[start_index
			+ num_elements_to_remove];
	args_array[total_args - num_elements_to_remove] = NULL;
	(*current_position)--;
}

int	verify_fd_perm(char *file_path)
{
	if (access(file_path, F_OK) == -1)
	{
		ft_putstr_fd(TERMINAL_NAME, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putstr_fd(file_path, STDERR_FILENO);
		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
		return (0);
	}
	return (1);
}

int	verify_fd_type(char *file_path)
{
	int		file_descriptor;
	char	buffer[1024];
	ssize_t	bytes_read;

	file_descriptor = open(file_path, O_RDONLY);
	if (file_descriptor == -1)
		return (0);
	bytes_read = read(file_descriptor, buffer, sizeof(buffer));
	close(file_descriptor);
	if (bytes_read == -1)
	{
		ft_putstr_fd(TERMINAL_NAME, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putstr_fd(file_path, STDERR_FILENO);
		ft_putstr_fd(": is a directory\n", STDERR_FILENO);
		return (0);
	}
	return (1);
}

int	handle_output_append(char **command_args, int *arg_index, int *output_fd)
{
	if (*output_fd != STDOUT_FILENO)
		close(*output_fd);
	if (ft_strncmp(command_args[*arg_index], ">", 2) == 0)
		*output_fd = open(command_args[*arg_index + 1],
				O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (ft_strncmp(command_args[*arg_index], ">>", 2) == 0)
		*output_fd = open(command_args[*arg_index + 1],
				O_CREAT | O_RDWR | O_APPEND, 0644);
	if (*output_fd == -1)
		return (ft_putstr_fd(TERMINAL_NAME, STDERR_FILENO), 0);
	return (remove_elements(command_args, arg_index, 2), 1);
}

int	execute_heredoc(int file_descriptor, char *delimiter, int *exit_code)
{
	char	*input_line;
	int		temp_variable;

	temp_variable = 0;
	file_descriptor = open("obj/heredoc", O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (file_descriptor == -1)
		return (ft_putstr_fd(TERMINAL_NAME, STDERR_FILENO), -1);
	while (1)
	{
		input_line = readline("> ");
		if (!input_line)
			break ;
		if (ft_strncmp(input_line, delimiter, ft_strlen(delimiter)) == 0)
		{
			free_str(input_line);
			break ;
		}
		perform_expansion(&input_line, &temp_variable);
		ft_putstr_fd(input_line, file_descriptor);
		ft_putstr_fd("\n", file_descriptor);
		free_str(input_line);
	}
	close(file_descriptor);
	file_descriptor = open("obj/heredoc", O_RDONLY);
	if (file_descriptor == -1)
		return (ft_putstr_fd(TERMINAL_NAME, STDERR_FILENO), -1);
	return (file_descriptor);
}

int	handle_input_heredoc(char **command_args, int *arg_index, int *input_fd,
		int *exit_code)
{
	int	temp_variable;

	temp_variable = 0;
	if (verify_fd_perm(command_args[*arg_index + 1]) == 0)
		return (*exit_code = 1, 0);
	else if (verify_fd_type(command_args[*arg_index + 1]) == 0)
		return (*exit_code = 126, 0);
	if (*input_fd != STDIN_FILENO)
		close(*input_fd);
	if (ft_strncmp(command_args[*arg_index], "<", 2) == 0)
		*input_fd = open(command_args[*arg_index + 1], O_RDONLY);
	if (ft_strncmp(command_args[*arg_index], "<<", 2) == 0)
		*input_fd = execute_heredoc(*input_fd, command_args[*arg_index + 1],
				exit_code);
	if (*input_fd == -1)
		return (ft_putstr_fd(TERMINAL_NAME, STDERR_FILENO), *exit_code = 1, 0);
	return (remove_elements(command_args, arg_index, 2), 1);
}

int	handle_redirections(char **command_args, int command_count,
		int *file_descriptors, int *exit_code)
{
	int	arg_index;
	int	cmd_index;

	cmd_index = 0;
	while (cmd_index < command_count)
	{
		arg_index = -1;
		while (command_args[cmd_index] && command_args[cmd_index][++arg_index])
		{
			if (ft_strchr(command_args[cmd_index][arg_index], '>')
				&& ft_strlen(command_args[cmd_index][arg_index]) <= 2)
			{
				if (!handle_output_append(command_args[cmd_index], &arg_index,
						&file_descriptors[cmd_index * 2 + 1]))
					return (*exit_code = 1, 0);
			}
			else if (ft_strchr(command_args[cmd_index][arg_index], '<')
				&& ft_strlen(command_args[cmd_index][arg_index]) <= 2)
			{
				if (!handle_input_heredoc(command_args[cmd_index], &arg_index,
						&file_descriptors[cmd_index * 2], exit_code))
					return (0);
			}
		}
		cmd_index++;
	}
	return (1);
}
