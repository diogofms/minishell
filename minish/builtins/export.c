/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: disilva <disilva@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 17:59:11 by disilva           #+#    #+#             */
/*   Updated: 2024/08/01 18:04:40 by disilva          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include "builtins.h"

int	env_cmp(char *current_env, char *new_env)
{
	int	i;

	i = 0;
	while (current_env[i] != '=' && current_env[i] == new_env[i])
		i++;
	if (current_env[i] == '=')
		return (1);
	return (0);
}

int	export(char **args, t_env_list *head, int fd)
{
	int		i;
	char	*new_var;

	if (args[1] == NULL)
	{
		while (head->next != NULL)
		{
			write (fd, "declare -x ", 11);
			write (fd, head->content, ft_strlen(head->content, 0));
			write (fd, "\n", 1);
			head = head->next;
		}
	}
	else
	{
		i = 1;
		while (args[i] != NULL)
		{
			new_var = malloc(strlen (args[i], '=') + 1);
			if (malloc_fail(new_var, strerror(errno)) == 1)
				return (1);
			str_cpy(args[i], &new_var, '=');
			while (head->next != NULL)
			{
				if (env_cmp(head->content, new_var) == 1)
				{
					free (head->content);
					head->content = malloc(ft_strlen(args[i], 0) + 1);
					if (malloc_fail(head->content, strerror(errno)) == 1)
						return (1);
					break ;
				}
				head = head->next;
			}
			if (head->next == NULL)
			{
				if (add_node(&(head->next), ft_strlen(args[i], 0)) == 1)
					return (1);
			}
			str_cpy(args[i], &head->content, 0);
		}
	}
	return (0);
}
