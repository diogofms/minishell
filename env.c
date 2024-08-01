/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: disilva <disilva@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 17:59:25 by disilva           #+#    #+#             */
/*   Updated: 2024/08/01 17:59:26 by disilva          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include "builtins.h"

int	env(t_env_list *head, int fd)
{
	if (fd > 1)
	{
		while (head->next != NULL)
		{
			write (fd, head->content, ft_strlen(head->content, 0));
			write (fd, "\n", 1);
			head = head->next;
		}
		return (0);
	}
}
