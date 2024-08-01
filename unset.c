/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: disilva <disilva@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 17:58:56 by disilva           #+#    #+#             */
/*   Updated: 2024/08/01 18:04:49 by disilva          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include <unistd.h>

t_env_list	*unset(char **args, t_env_list *head)
{
	int			i;
	int			j;
	t_env_list	*prev;
	t_env_list	*node;

	prev = head;
	node = head;
	i = 1;
	while (args[i] != NULL)
	{
		while (node->next != NULL)
		{
			j = 0;
			while (node->content[j] != '=')
			{
				if (node->content[j] != args[i][j])
					break ;
				j++;
			}
			if (node->content[j] == '=')
			{
				free (node->content);
				if (node == prev)
					head = node->next;
				else
					prev->next = node->next;
				free (node);
				return (head);
			}
			else
			{
				prev = node;
				node = node->next;
			}
		}
		i++;
	}
	return (head);
}
