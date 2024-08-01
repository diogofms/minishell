/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: disilva <disilva@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 17:59:30 by disilva           #+#    #+#             */
/*   Updated: 2024/08/01 17:59:30 by disilva          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "builtins.h"

void	echo_export(int fd, t_env_list *head)
{
	while (head->next != NULL)
	{
		write (fd, "declare -x ", 11);
		write (fd, head->content, ft_strlen(head->content, 0));
		write (fd, " ", 1);
		head = head->next;
	}
}

void	echo(char **args, int fd, t_env_list *head)
{
	int		i;
	int		j;
	int		extra_line;
	char	*str;

	extra_line = 1;
	i = 1;
	if (args[1] == "-n")
	{
		extra_line = 0;
		i = 2;
	}
	while (args[i] != 0)
	{
		if (ft_strcmp(args[i], "$(pwd)") == 1)
			pwd(fd);
		else if (ft_strcmp(args[i], "$(env)") == 1)
			env(head, fd);
		else if (ft_strcmp(args[i], "$(export)") == 1)
			echo_export(fd, head);
		else
		{
			write (fd, args[i], ft_strlen(args[i], 0));
			if (args[i + 1] != 0)
				write (fd, " ", 1);
		}
		i++;
	}
	write (1, "\n", 1);
	if (extra_line == 1)
		write (fd, "\n", 1);
}
