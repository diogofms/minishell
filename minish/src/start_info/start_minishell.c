/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_minishell.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abekri <abekri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 06:34:42 by abekri            #+#    #+#             */
/*   Updated: 2024/08/04 04:10:29 by abekri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_var_set(char **env, char *var)
{
	int	i;

	i = 0;
	while (env[i] != NULL && ft_strncmp(env[i], var, ft_strlen(var)) != 0)
		i++;
	return (env[i] != NULL);
}

int	find_var_index(char **env, char *var)
{
	int		i;
	size_t	len;

	i = 0;
	len = ft_strlen(var);
	while (env[i] != NULL)
	{
		if (ft_strncmp(env[i], var, len) == 0 && (env[i][len] == '='
			|| env[i][len] == '\0' || (env[i][len] == '+' && env[i][len
					+ 1] == '=')))
			return (i);
		i++;
	}
	return (-1);
}

int	has_prefix(char *env[], char *s)
{
	int	i;

	i = -1;
	while (env[++i])
	{
		if (ft_strncmp(env[i], s, ft_strlen(s)) == 0)
			return (1);
	}
	return (0);
}

void	copy_env_var(char *orig_env[], char *mini_env[], int *i)
{
	if (has_prefix(orig_env, "OLDPWD=")
		&& ft_strncmp(orig_env[*i], "OLDPWD", 6) == 0)
	{
		null_check_free_str(mini_env[*i]);
		mini_env[*i] = ft_strdup("OLDPWD");
	}
	else
	{
		mini_env[*i] = ft_strdup(orig_env[*i]);
	}
	(*i)++;
}

void	add_missing_vars(char *mini_env[], int *i)
{
	if (!find_var_index(mini_env, "PWD"))
	{
		mini_env[(*i)++] = ft_strjoin("PWD=", getcwd(NULL, 0));
	}
	if (!find_var_index(mini_env, "OLDPWD"))
	{
		mini_env[(*i)++] = ft_strdup("OLDPWD");
	}
	if (!find_var_index(mini_env, "SHLVL"))
	{
		mini_env[(*i)++] = ft_strdup("SHLVL=1");
		mini_env[*i] = NULL;
	}
	else
	{
		adjust_shell_evl(mini_env, 1);
		mini_env[*i] = NULL;
	}
}

void	setup_env(char *orig_env[], char *mini_env[], int count)
{
	int	i;

	i = 0;
	while (orig_env[i] && i < count)
	{
		copy_env_var(orig_env, mini_env, &i);
	}
	add_missing_vars(mini_env, &i);
}


void	free_mini_env(char **mini_env)
{
	if (mini_env)
		free_2d_array(mini_env);
}

void	init_env(char **env, char **mini_env, int count)
{
	int	i;

	i = count;
	if (!is_var_set(env, "PWD"))
		count++;
	if (!is_var_set(env, "OLDPWD"))
		count++;
	if (!is_var_set(env, "SHLVL"))
		count++;
	mini_env = (char **)malloc(sizeof(char *) * (count + 2));
	if (!mini_env)
	{
		printf("Error: malloc failed\n");
		free_mini_env(*mini_env);
		exit(1);
	}
	setup_env(env, mini_env, count);
}

int	count_env_vars(char **env)
{
	int	len;

	len = 0;
	while (env[len])
		len++;
	return (len);
}

char	*get_cwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd == NULL)
		return (ft_strdup(""));
	return (cwd);
}

char	*get_home(void)
{
	char	*cwd;
	char	*home;
	char	**parts;
	char	*tmp;

	cwd = getcwd(NULL, 0);
	if (cwd == NULL)
		return (ft_strdup(""));
	parts = ft_split(cwd, '/');
	tmp = ft_strdup("/");
	home = ft_strjoin(tmp, parts[0]);
	free(tmp);
	tmp = ft_strjoin(home, "/");
	free(home);
	home = ft_strjoin(tmp, parts[1]);
	free(tmp);
	free_2d(parts);
	if (home == NULL)
		return (free_str(cwd), ft_strdup(""));
	free_str(cwd);
	return (home);
}

void	start_info_ms(char **env, char **term_name, char **mini_env,
		int *exit_code)
{
	int		len;
	char	*path;
	char	*home;

	len = count_env_vars(env);
	init_env(env, mini_env, len);
	*exit_code = 0;
	*term_name = (char *)malloc(MAX_STRING_LEN * sizeof(char));
	path = get_cwd();
	home = get_home();
	if (DEBUG)
	{
		print_2d_array(mini_env, "ENVIRONMENT VARIABLES");
		printf("%s", format_iterm(mini_env));
	}
	else
	{
		printf("minishell-0.1$ ");
	}
	free(path);
	free(home);
}
