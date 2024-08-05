/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: disilva <disilva@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 17:59:35 by disilva           #+#    #+#             */
/*   Updated: 2024/08/01 18:03:27 by disilva          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include "builtins.h"

int	update_oldpwd(t_env_list *head)
{
	char	*current_dir;
	int		j;
	int		k;

	if (getcwd(current_dir, 4096) == NULL)
	{
		printf ("%s\n", strerror(errno));
		return (1);
	}
	while (head->next != NULL)
	{
		if (env_cmp(head->content, "OLDPWD") == 1)
			break ;
		head = head->next;
	}
	if (head->next != NULL)
		free(head->content);
	else
	{
		if (add_node(&(head->next),
				(ft_strlen("OLDPWD ", 0) + ft_strlen(current_dir, 0))) == 1)
			return (1);
		head = head->next;
	}
	head->content = strcnc("OLDPWD ", current_dir);
	if (head->content == NULL)
		return (1);
	return (0);
}

int	cd(char **args, t_env_list *head)
{
	int				i;
	int				j;
	int				k;
	DIR				*dir;
	struct dirent	*entry;
	char			cwd[4096];
	char			*str;
	char			ogdir[4096];

	str = malloc (4096);
	if (malloc_fail(str, strerror(errno)) == 1)
		return (1);
	if (getcwd(ogdir, 4096) == NULL)
	{
		free(str);
		printf ("%s\n", strerror(errno));
		return (1);
	}
	i = 0;
	if (ft_strcmp(args[0], "/") == 1)
	{
		if ((chdir("/")) != 0)
		{
			free(str);
			printf ("%s\n", strerror(errno));
			return (1);
		}
		i++;
	}
	else if (ft_strcmp(args[0], "-") == 1)
	{
		while (head->next != NULL)
		{
			if (env_cmp(head->content, "OLDPWD") == 1)
				break ;
			head = head->next;
		}
		if (head->next == NULL)
		{
			free(str);
			printf("bash: cd: /path/to/prev");
			printf("ious/directory: No such file or directory\n");
			return (1);
		}
		j = 0;
		while (head->content[j] != '=')
			j++;
		j++;
		k = 0;
		while (head->content[j] != 0)
		{
			str[k] = head->content[j];
		}
		if ((chdir(str)) != 0)
		{
			free(str);
			printf ("%s\n", strerror(errno));
			return (1);
		}
	}
	else if (args[0] == "~")
	{
		str = getenv("HOME");
		if ((chdir(str)) != 0)
		{
			free(str);
			printf ("%s\n", strerror(errno));
			return (1);
		}
		i++;
	}
	while (args[i] != 0)
	{
		if (args[i] == "..")
		{
			if (getcwd(cwd, 4096) == NULL)
			{
				free(str);
				printf ("%s\n", strerror(errno));
				return (1);
			}
			j = 0;
			while (cwd[j] != 0)
				j++;
			while (cwd[j] != '/')
				j--;
			k = 0;
			while (k != j)
			{
				str[k] = cwd[k];
				k++;
			}
			str[k] = 0;
			if ((chdir(str)) != 0)
			{
				free(str);
				printf ("%s\n", strerror(errno));
				return (1);
			}
		}
		else
		{
			dir = opendir(".");
			if (dir == NULL)
			{
				free(str);
				printf ("%s\n", strerror(errno));
				return (1);
			}
			entry = readdir(dir);
			while (entry != NULL)
			{
				if (ft_strcmp(entry->d_name, args[i]) == 1)
				{
					if ((chdir(entry->d_name)) != 0)
					{
						free(str);
						printf ("%s\n", strerror(errno));
						return (1);
					}
					break ;
				}
				entry = readdir(dir);
			}
			closedir(dir);
			if (entry == NULL)
			{
				if ((chdir(ogdir)) != 0)
				{
					free(str);
					printf ("%s\n", strerror(errno));
					return (1);
				}
				printf ("bash: cd: ");
				i = 0;
				while (args[i] != 0)
				{
					printf ("%s", args[i]);
					if (args[i + 1] != 0)
						printf ("/");
				}
				free(str);
				printf (": No such file or directory\n");
				return (1);
			}
		}
		i++;
	}
	free(str);
	if (update_oldpwd(head) == 1)
		return (1);
	return (0);
}
