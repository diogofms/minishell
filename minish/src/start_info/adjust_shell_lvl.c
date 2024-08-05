/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   adjust_shell_lvl.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abekri <abekri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 04:23:02 by abekri            #+#    #+#             */
/*   Updated: 2024/08/02 15:52:40 by abekri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	free_shell(char **env, char *s)
{
	int	ind;

	ind = 0;
	while (env[ind])
	{
		if (ft_strncmp(env[ind], "SHLVL=", 6) == 0)
		{
			free(env[ind]);
			env[ind] = s;
			return ;
		}
		ind++;
	}
	env[ind] = s;
	env[ind + 1] = NULL;
}

char	*bring_env(char **env, char const *n)
{
	int		ind;
	size_t	n_size;

	ind = 0;
	n_size = ft_strlen(n);
	while (env[ind])
	{
		if (ft_strncmp(env[ind], n, n_size) == 0 && env[ind][n_size] == '=')
		{
			return (env[ind]);
		}
		ind++;
	}
	return (NULL);
}

char	*extract_value(char *s)
{
	char	*val;
	int		ind;

	ind = 0;
	if (!s)
		return (NULL);
	while (s[ind] != '=')
		ind++;
	val = ft_strdup(s + ind + 1);
	free_str(s);
	return (val);
}

int	check_numeric_flag(char *s)
{
	int	numeric_flag;
	int	ind;

	numeric_flag = 1;
	ind = 0;
	while (s[ind] != '\0')
	{
		if (s[ind] < '0' || s[ind] > '9')
		{
			numeric_flag = 0;
			break ;
		}
		ind++;
	}
	return (numeric_flag);
}

int	check_alpha_flag(char *s)
{
	int	alpha_flag;
	int	ind;

	alpha_flag = 0;
	ind = 0;
	while (s[ind] != '\0')
	{
		if (ft_isalpha(s[ind]))
		{
			alpha_flag = 1;
			break ;
		}
		ind++;
	}
	return (alpha_flag);
}

int	handle_numeric_flag(char *s)
{
	if (ft_strcmp(s, "0") == 0)
	{
		return (1);
	}
	else if (ft_strcmp(s, "1") == 0)
	{
		return (2);
	}
	else
	{
		return (0);
	}
}

int	validate_string_content(char *s)
{
	int	numeric_flag;
	int	alpha_flag;

	numeric_flag = check_numeric_flag(s);
	alpha_flag = check_alpha_flag(s);
	if (numeric_flag)
	{
		return (handle_numeric_flag(s));
	}
	else
	{
		if (alpha_flag)
		{
			return (2);
		}
		else
		{
			return (1);
		}
	}
}

char	*get_old_shell_level(char **env)
{
	return (bring_env(env, "SHLVL"));
}

char	*get_new_level_num(char *old_lvl_num, int increment)
{
	int	old_lvl_val;
	int	new_lvl_val;

	if (validate_string_content(old_lvl_num) == 1)
		return (ft_strdup("0"));
	if (validate_string_content(old_lvl_num) == 2)
		return (ft_strdup("1"));
	old_lvl_val = ft_atoi(old_lvl_num);
	if (increment == 0)
	{
		new_lvl_val = old_lvl_val - 1;
	}
	else
	{
		new_lvl_val = old_lvl_val + 1;
	}
	if (new_lvl_val < 0)
		new_lvl_val = 0;
	return (ft_itoa(new_lvl_val));
}

void	update_env_with_new_level(char **env, char *new_lvl_str)
{
	int	ind;

	ind = 0;
	while (env[ind] && ft_strncmp(env[ind], "SHLVL=", 6) != 0)
		ind++;
	if (env[ind])
		free(env[ind]);
	env[ind] = new_lvl_str;
	if (!env[ind + 1])
		env[ind + 1] = NULL;
}

void	adjust_shell_lvl(char **env, int increment)
{
	char	*old_lvl_str;
	char	*old_lvl_num;
	char	*new_lvl_num;
	char	*new_lvl_str;

	old_lvl_str = get_old_shell_level(env);
	if (!old_lvl_str)
		return ;
	old_lvl_num = extract_value(old_lvl_str);
	if (!old_lvl_num)
		return ;
	new_lvl_num = get_new_level_num(old_lvl_num, increment);
	new_lvl_str = ft_strjoin("SHLVL=", new_lvl_num);
	update_env_with_new_level(env, new_lvl_str);
	free_str(old_lvl_str);
	free_str(old_lvl_num);
	free_str(new_lvl_num);
	free_shell(env, new_lvl_str);
}
