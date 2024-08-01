/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enviromental_variables.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: disilva <disilva@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 17:59:20 by disilva           #+#    #+#             */
/*   Updated: 2024/08/01 18:03:45 by disilva          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include "builtins.h"

void	ft_free_list(t_env_list *head)
{
	t_env_list	*next;

	if (head == NULL)
		return (0);
	while (head->next != NULL)
	{
		next = head->next;
		free (head->content);
		free(head);
		head = next;
	}
	if (head->content != NULL)
		free (head->content);
	free (head);
	return (0);
}

t_env_list	*ft_envp(char **envp, t_env_list *head)
{
	t_env_list	*current;
	int			j;
	int			k;

	head = malloc(sizeof(t_env_list));
	if (head == NULL)
	{
		printf("%s\n", strerror(errno));
		exit (0);
	}
	current = head;
	k = 0;
	while (envp[k] != NULL)
	{
		j = 0;
		while (envp[k][j] != 0)
			j++;
		current->content = malloc((j + 1) * sizeof(char));
		if (current->content == NULL)
		{
			printf("%s\n", strerror(errno));
			ft_free_list(head);
			exit (0);
		}
		j = 0;
		while (envp[k][j] != 0)
		{
			current->content[j] = envp[k][j];
			j++;
		}
		current->content[j] = 0;
		k++;
		if (envp[k] != NULL)
		{
			current->next = malloc(sizeof(t_env_list));
			if (current->next == NULL)
			{
				printf("%s\n", strerror(errno));
				ft_free_list(head);
				exit (0);
			}
			current = current->next;
		}
	}
	current->next = NULL;
	return (head);
}
