/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abekri <abekri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 03:34:28 by abekri            #+#    #+#             */
/*   Updated: 2024/07/26 16:51:43 by abekri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	add_cmd_node(char ***cmds, int *cmd_count)
{
	char	**new_cmds;
	int		i;

	(*cmd_count)++;
	new_cmds = malloc(sizeof(char *) * (*cmd_count + 1));
	if (!new_cmds)
		return (0);
	i = 0;
	while (i < *cmd_count - 1)
	{
		new_cmds[i] = (*cmds)[i];
		i++;
	}
	free(*cmds);
	*cmds = new_cmds;
	(*cmds)[*cmd_count] = NULL;
	return (1);
}

int	create_args_array(char **tokens, char ***args, int arg_count)
{
	int	i;

	i = 0;
	*args = malloc(sizeof(char *) * (arg_count + 1));
	if (!*args)
		return (0);
	while (i < arg_count)
	{
		if (ft_strcmp(tokens[i], "<") == 0)
			(*args)[i] = ft_strdup("<");
		else if (ft_strcmp(tokens[i], ">") == 0)
			(*args)[i] = ft_strdup(">");
		else if (ft_strcmp(tokens[i], ">>") == 0)
			(*args)[i] = ft_strdup(">>");
		else if (ft_strcmp(tokens[i], "<<") == 0)
			(*args)[i] = ft_strdup("<<");
		else
			(*args)[i] = ft_strdup(tokens[i]);
		if (!(*args)[i])
			return (0);
		i++;
	}
	(*args)[arg_count] = NULL;
	return (1);
}

int	initialize_args(char **tokens, int token_count, char ***cmds, int cmd_index)
{
	int		arg_count;
	int		i;
	char	**args;

	arg_count = 0;
	i = 0;
	args = NULL;
	while (i < token_count && ft_strcmp(tokens[i], "|") != 0)
	{
		arg_count++;
		i++;
	}
	if (arg_count == 0)
		return (1);
	if (!create_args_array(tokens, &args, arg_count))
		return (0);
	(*cmds)[cmd_index] = args;
	return (1);
}

void	print_array(char **array, char *msg)
{
	int	i;

	if (!array)
		return ;
	printf("%s\n", msg);
	i = 0;
	while (array[i] != NULL)
	{
		printf("  %s\n", array[i]);
		i++;
	}
}

void	print_commands(char **cmds, int cmd_count, char *str_msg)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	if (cmd_count == 0 || cmds == NULL)
	{
		printf("The command list is empty or NULL\n");
		return ;
	}
	while (i < cmd_count)
	{
		printf("Arguments for command %d:\n", i + 1);
		if (cmds[i])
			print_array(&cmds[i], str_msg);
		else
			printf("  args: NULL\n");
		printf("fd_infile: %d\n", STDIN_FILENO);
		printf("fd_outfile: %d\n", STDOUT_FILENO);
		if (i < cmd_count - 1)
			printf("Next command: %p\n", (void *)&cmds[i + 1]);
		else
			printf("Next command: NULL\n");
		if (i > 0)
			printf("Previous command: %p\n", (void *)&cmds[i - 1]);
		else
			printf("Previous command: NULL\n");
		i++;
	}
}

int	generate_abstract_cmds(char **cmds, int token_count, char **envp, int *exit_cd)
{
	int	cmd_count;
	int	i;

	cmd_count = 0;
	i = 0;
	while (i < token_count)
	{
		if (i == 0 || ft_strcmp(cmds[i - 1], "|") == 0)
		{
			if (!add_cmd_node(&cmds, &cmd_count))
				return (0);
		}
		if (ft_strcmp(cmds[i], "|") != 0)
		{
			if (!initialize_args(cmds, token_count, &cmds, cmd_count - 1))
				return (0);
		}
		i++;
	}
	if (DEBUG)
		print_commands(cmds, cmd_count, "---AFTER---");
	return (1);
}
