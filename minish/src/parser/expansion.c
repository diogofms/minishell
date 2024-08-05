/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abekri <abekri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 02:00:04 by abekri            #+#    #+#             */
/*   Updated: 2024/08/01 20:02:53 by abekri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return (*(const unsigned char *)s1 - *(const unsigned char *)s2);
}

void	free_str(char *str)
{
	if (str)
		free(str);
}

int	check_valid_env_char(char *str)
{
	return (ft_isalnum(*str) || ft_isalpha(*str) || *str == '_');
}

int	handle_exit_status(int exit_code, int *i, char **expanded_str)
{
	char	*exit_status;
	char	*new_expanded_str;

	exit_status = ft_itoa(exit_code);
	if (!exit_status)
		return (0);
	new_expanded_str = ft_strncat(*expanded_str,
			exit_status, ft_strlen(exit_status));
	free(exit_status);
	if (!new_expanded_str)
		return (0);
	*expanded_str = new_expanded_str;
	(*i)--;
	return (1);
}

char	*extract_env_value(char **var_env_names, char *var_name)
{
	int	var_len;
	int	index;

	var_len = ft_strlen(var_name);
	index = 0;
	while (var_env_names && var_env_names[index])
	{
		if (ft_strncmp(var_env_names[index], var_name, var_len) == 0
			&& var_env_names[index][var_len] == '=')
		{
			return (ft_substr(var_env_names[index] + var_len + 1, 0,
					ft_strlen(var_env_names[index] + var_len + 1)));
		}
		index++;
	}
	return (NULL);
}

int	handle_env_variable(char **args, int *i, char **expanded_str, char **envp)
{
	char	*env_var;
	char	*var_env_value;
	int		start;
	int		end;
	char	*new_expanded_str;

	start = *i;
	var_env_value = NULL;
	env_var = NULL;
	while (args[*i] && check_valid_env_char(args[*i]))
		(*i)++;
	end = *i;
	env_var = ft_substr(args[start], 0, end - start);
	if (!env_var)
		return (0);
	var_env_value = extract_env_value(envp, env_var);
	free(env_var);
	if (!var_env_value)
		return (0);
	new_expanded_str = ft_strncat(*expanded_str, var_env_value, ft_strlen(var_env_value));
	free(var_env_value);
	if (!new_expanded_str)
		return (0);
	*expanded_str = new_expanded_str;
	(*i)--;
	return (1);
}

int	expand_variable(char **args, int *i, char **expanded_str, char **envp)
{
	if (args[*i] && (ft_strcmp(args[*i], "?") == 0 || ft_strcmp(args[*i],
				"$") == 0))
	{
		if (ft_strcmp(args[*i], "?") == 0)
		{
			if (!handle_exit_status(0, i, expanded_str))
				return (0);
		}
		else if (ft_strcmp(args[*i], "$") == 0)
		{
			if (!handle_exit_status(95394, i, expanded_str))
				return (0);
		}
	}
	else
	{
		if (!handle_env_variable(args, i, expanded_str, envp))
			return (0);
	}
	return (1);
}

char	*ft_strncat(char *s1, char *s2, int n)
{
	char	*str;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!s1 && !s2)
		return (NULL);
	str = (char *)malloc((ft_strlen(s1) + n + 1) * sizeof(char));
	if (!str)
		return (NULL);
	while (s1 && s1[i] != '\0')
	{
		str[i] = s1[i];
		i++;
	}
	while (j < n && s2 && s2[j] != '\0')
		str[i++] = s2[j++];
	str[i] = '\0';
	free_str(s1);
	return (str);
}

void	handle_double_quotes(char **args, int *i, char **expanded_str, char **envp)
{
	while (args && args[*i] && ft_strcmp(args[*i], "\"") != 0)
	{
		if (args[*i] && ft_strcmp(args[*i], "$") == 0 && ft_strcmp(args[*i + 1],
				" ") != 0 && ft_strcmp(args[*i + 1], "\"") != 0)
		{
			expand_variable(args, i, expanded_str, envp);
		}
		else
		{
			*expanded_str = ft_strncat(*expanded_str, args[*i], ft_strlen(args[*i]));
		}
		(*i)++;
	}
}

char	*handle_single_quotes(char *arg, int *i, char **expanded_str)
{
	int	start;
	int	end;

	start = ++(*i);
	while (arg[*i] && ft_strncmp(arg + *i, "'", 1) != 0)
		(*i)++;
	end = *i;
	(*i)++;
	*expanded_str = ft_strncat(*expanded_str, arg + start, end - start);
	return (*expanded_str);
}

char	*successful_expansion(char **envp, char *arg, char **expanded_str,
		int *exp_flag)
{
	int		i;
	char	*args[2];

	args[0] = arg;
	args[1] = NULL;
	i = 0;
	while (arg[i])
	{
		if (ft_strncmp(arg + i, "'", 1) == 0)
		{
			handle_single_quotes(arg, &i, expanded_str);
		}
		else if (ft_strncmp(arg + i, "\"", 1) == 0)
		{
			handle_double_quotes(args, &i, expanded_str, envp);
		}
		else if (ft_strncmp(arg + i, "$", 1) == 0 && arg[i + 1] != '\0')
		{
			if (expand_variable(args, &i, expanded_str, envp))
				*exp_flag = 1;
		}
		else
		{
			*expanded_str = ft_strncat(*expanded_str, arg + i, 1);
			i++;
		}
	}
	return (*expanded_str);
}

int	needs_expansion(char **envp, char *arg, int *exp_flag)
{
	char	*expanded_str;

	expanded_str = NULL;
	if (successful_expansion(envp, arg, &expanded_str, exp_flag))
	{
		free_str(arg);
		arg = expanded_str;
		return (1);
	}
	return (0);
}

void	free_2d_array(char **array)
{
	char	**tmp;

	if (array == NULL)
		return ;
	tmp = array;
	while (*tmp)
	{
		free(*tmp);
		tmp++;
	}
	free(array);
}

char	**combine_arrays(char **array_1, char **array_2, int start,
		int *exp_flag)
{
	int		i;
	int		j;
	int		len;
	char	**new_array;

	i = 0;
	j = 0;
	len = 0;
	while (array_1[len])
		len++;
	while (array_2[j])
		len++;
	new_array = (char **)malloc((len + 1) * sizeof(char *));
	if (!new_array)
		return (NULL);
	i = 0;
	while (array_1[i])
	{
		new_array[i] = ft_strdup(array_1[i]);
		if (!new_array[i])
		{
			free_2d_array(new_array);
			return (NULL);
		}
		i++;
	}
	j = 0;
	while (array_2[j])
	{
		new_array[i] = ft_strdup(array_2[j]);
		if (!new_array[i])
		{
			free_2d_array(new_array);
			return (NULL);
		}
		i++;
		j++;
	}
	new_array[i] = NULL;
	free_2d_array(array_1);
	free_2d_array(array_2);
	*exp_flag = 0;
	return (new_array);
}

void	remove_element(char **cmds, int *j)
{
	int	tmp;
	int	i;

	tmp = -1;
	i = *j - 1;
	while (*(cmds + ++tmp))
		;
	if (*(cmds + *j))
		free_str(*(cmds + *j));
	while (++i < tmp - 1)
		*(cmds + i) = *(cmds + i + 1);
	*(cmds + tmp - 1) = NULL;
	(*j)--;
}

void	print_2d_array(char **arr, char *msg)
{
	int	i;

	i = 0;
	printf("\n%s\n", msg);
	while (arr[i])
	{
		printf("%p [%d] ----- %s\n", (arr + i), i, arr[i]);
		i++;
	}
}

int	expansion_success(char **envp, char *input, int *fds, int *exit_cd)
{
	int		index;
	int		j;
	int		exp_flag;
	char	**cmds;

	cmds = ft_split(input, '|');
	index = 0;
	while (cmds[index])
	{
		if (DEBUG)
			print_2d_array(cmds, "before expansion");
		j = -1;
		exp_flag = 0;
		while (cmds[index][++j])
		{
			if (!needs_expansion(envp, &(cmds[index][j]), &exp_flag))
				remove_element(cmds, &j);
			if (exp_flag)
				cmds = combine_arrays(cmds,
						ft_split(cmds[index] + j, ' '), index, &exp_flag);
		}
		if (DEBUG)
			print_2d_array(cmds, "after expansion");
		index++;
	}
	return (1);
}
