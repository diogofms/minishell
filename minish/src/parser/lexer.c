/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abekri <abekri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 21:47:56 by skhastag          #+#    #+#             */
/*   Updated: 2024/07/23 07:23:27 by abekri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	**start_tokens(void)
{
	char	**tokens;

	tokens = (char **)malloc(sizeof(char *));
	if (tokens)
		tokens[0] = NULL;
	return (tokens);
}

char	**append_token_to_array(char **tokens, char *str)
{
	int		i;
	char	**new_tokens;

	i = 0;
	while (tokens && tokens[i])
		i++;
	new_tokens = (char **)malloc(sizeof(char *) * (i + 2));
	if (!new_tokens)
		return (NULL);
	i = 0;
	while (tokens && tokens[i])
	{
		new_tokens[i] = tokens[i];
		i++;
	}
	new_tokens[i] = str;
	new_tokens[i + 1] = NULL;
	free(tokens);
	return (new_tokens);
}

int	process_tokens(char *readline, char **tokens, int *info)
{
	char	*token_vals[1];
	int		token_type;

	token_type = get_next_token(readline, info, &info[3], token_vals);
	if (token_type == EOL)
	{
		return (0);
	}
	tokens = append_token_to_array(tokens, ft_strdup(token_vals[0]));
	if (!tokens)
	{
		return (-1);
	}
	return (1);
}

int	perform_lexical_analysis(char *readline, char ***cmds, int *fds,
		int *exit_cd)
{
	char	**tokens;
	int		info[4];
	int		i;
	int		result;

	i = 0;
	while (i < 4)
	{
		info[i] = 0;
		i++;
	}
	(void)fds;
	(void)exit_cd;
	tokens = start_tokens();
	if (!tokens)
	{
		return (0);
	}
	while (1)
	{
		result = process_tokens(readline, tokens, info);
		if (result != 1)
		{
			break ;
		}
	}
	if (result == -1)
	{
		return (0);
	}
	*cmds = tokens;
	return (1);
}
