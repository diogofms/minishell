/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abekri <abekri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 00:45:05 by abekri            #+#    #+#             */
/*   Updated: 2024/07/25 03:11:21 by abekri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_is_space(char c)
{
	return (c == 32 || (c >= 9 && c <= 13));
}

int	build_token(int type, char *val, int *token_info, char **token_vals)
{
	int	*token_types;
	int	*token_next;
	int	*token_prev;
	int	*token_count;

	token_types = &token_info[0];
	token_next = &token_info[MAX_TOKENS];
	token_prev = &token_info[2 * MAX_TOKENS];
	token_count = &token_info[3 * MAX_TOKENS];
	if (*token_count >= MAX_TOKENS)
		return (-1);
	token_types[*token_count] = type;
	if (type == '|')
		token_vals[*token_count] = ft_strdup("|");
	else if (type == R_DIR_APPEND)
		token_vals[*token_count] = ft_strdup(">>");
	else if (type == H_DOC)
		token_vals[*token_count] = ft_strdup("<<");
	else if (type == EOL)
		token_vals[*token_count] = ft_strdup("EOL");
	else
		token_vals[*token_count] = val;
	if (*token_count < MAX_TOKENS - 1)
		token_next[*token_count] = *token_count + 1;
	else
		token_next[*token_count] = -1;
	if (*token_count > 0)
		token_prev[*token_count] = *token_count - 1;
	else
		token_prev[*token_count] = -1;
	return ((*token_count)++);
}

int	extract_token_word(char *line, int *pos_info, int *token_info,
		char **token_vals)
{
	int		*position;
	int		*isdouble_quote;
	int		*is_single_quote;
	int		start;
	char	*word;

	position = &pos_info[0];
	isdouble_quote = &pos_info[1];
	is_single_quote = &pos_info[2];
	start = *position;
	while (line[*position])
	{
		if (line[*position] == (char) '\'' || line[*position] == (char) '\"')
			manage_quotes(line, pos_info);
		if (!*isdouble_quote && ft_is_space(line[*position])
			&& !*is_single_quote)
			break ;
		if (!*is_single_quote && (line[*position] == (char) '|'
				|| line[*position] == (char) '<'
				|| line[*position] == (char) '>')
			&& !*isdouble_quote)
			break ;
		(*position)++;
	}
	if (*isdouble_quote || *is_single_quote)
		return (build_token(ERR, NULL, token_info, token_vals));
	word = ft_calloc(1, (*position - start + 1));
	ft_strlcpy(word, line + start, *position - start + 1);
	return (build_token(WORD, word, token_info, token_vals));
}

void	manage_quotes(char *line, int *pos_info)
{
	int	*position;
	int	*isdouble_quote;
	int	*is_single_quote;

	position = &pos_info[0];
	isdouble_quote = &pos_info[1];
	is_single_quote = &pos_info[2];
	if (line[*position] == (char) '\'' && line[*position - 1] != (char) '\\'
		&& !*isdouble_quote && !*is_single_quote)
		*is_single_quote = 1;
	else if (line[*position] == (char) '\''
		&& line[*position - 1] != (char) '\\'
		&& !*isdouble_quote && *is_single_quote)
		*is_single_quote = 0;
	else if (line[*position] == (char) '\"'
		&& line[*position - 1] != (char) '\\'
		&& !*isdouble_quote && !*is_single_quote)
		*isdouble_quote = 1;
	else if ((line[*position] == (char) '\"')
		&& (line[*position - 1] != (char) '\\')
		&& *isdouble_quote && !*is_single_quote)
		*isdouble_quote = 0;
}

void	free_token_list(char **token_vals, int token_count)
		// must be used in execution_manager by my partner which will free the token list of arrays
{
	int	i;

	i = 0;
	while (i < token_count)
	{
		if (token_vals[i])
		{
			free(token_vals[i]);
		}
		if (token_vals[i] == NULL)
		{
			break ;
		}
		i++;
	}
	free(token_vals);
}

static void	skip_spaces(char *line, int *position)
{
	while (ft_is_space(line[*position]))
	{
		(*position)++;
	}
}

static int	handle_single_char_token(char *line, int *position, int *token_info,
		char **token_vals)
{
	char	current_char;

	current_char = line[*position];
	if (current_char == '|' || current_char == '<' || current_char == '>')
	{
		(*position)++;
		return (build_token(current_char, NULL, token_info, token_vals));
	}
	return (0);
}

static int	handle_double_char_token(char *line, int *position, int *token_info,
		char **token_vals)
{
	char	token_type;

	if ((line[*position] == '<' && line[*position + 1] == '<')
		|| (line[*position] == '>' && line[*position + 1] == '>'))
	{
		if (line[*position] == '<')
			token_type = H_DOC;
		else
			token_type = R_DIR_APPEND;
		*position += 2;
		return (build_token(token_type, NULL, token_info, token_vals));
	}
	return (0);
}

int	get_next_token(char *line, int *pos_info, int *token_info,
		char **token_vals)
{
	int	*position;

	position = &pos_info[0];
	while (*position < (int)ft_strlen(line))
	{
		skip_spaces(line, position);
		if (handle_single_char_token(line, position, token_info, token_vals))
		{
			return (1);
		}
		if (handle_double_char_token(line, position, token_info, token_vals))
		{
			return (1);
		}
		if (!ft_is_space(line[*position]))
		{
			return (extract_token_word(line, pos_info, token_info, token_vals));
		}
	}
	return (build_token(EOL, NULL, token_info, token_vals));
}
