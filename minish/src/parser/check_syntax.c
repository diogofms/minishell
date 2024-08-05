/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abekri <abekri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 02:32:54 by abekri            #+#    #+#             */
/*   Updated: 2024/07/29 02:16:13 by abekri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	check_pipe_err(int *tokens, int count, int *exit_cd)
{
	int	i;

	i = 0;
	while (i < count)
	{
		if (tokens[i] == '|')
		{
			if (i == 0 || i == count - 1 || tokens[i - 1] == '|'
				|| tokens[i + 1] == '|' || tokens[i + 1] == EOL)
			{
				dup2(STDERR_FILENO, STDOUT_FILENO);
				printf(ERR_SYNTX_MSG, '|');
				dup2(STDOUT_FILENO, STDERR_FILENO);
				*exit_cd = 2;
				return (0);
			}
		}
		i++;
	}
	return (1);
}

int	check_redir_err(int *tokens, int count, char **values, int *exit_cd)
{
	int	i;

	i = 0;
	while (i < count)
	{
		if (tokens[i] == '<' || tokens[i] == '>'
			|| tokens[i] == R_DIR_APPEND || tokens[i] == H_DOC)
		{
			if ((i == count - 1 || tokens[i + 1] != WORD)
				|| (i == 0 && tokens[i + 1] != WORD))
			{
				dup2(STDERR_FILENO, STDOUT_FILENO);
				printf(ERR_SYNTX_MSG_RE_H, "newline");
				dup2(STDOUT_FILENO, STDERR_FILENO);
				*exit_cd = 2;
				return (0);
			}
		}
		i++;
	}
	return (1);
}

int	check_syntax(int *tokens, char **values, int count, int *exit_cd)
{
	int	i;

	i = 0;
	while (i < count)
	{
		if (!check_pipe_err(&tokens[i], count, exit_cd)
			|| !check_redir_err(&tokens[i], count, &values[i], exit_cd))
		{
			return (free(tokens), free(values), 0);
		}
		i++;
	}
	return (1);
}
