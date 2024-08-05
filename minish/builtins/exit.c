/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abekri <abekri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 17:59:16 by disilva           #+#    #+#             */
/*   Updated: 2024/08/02 21:29:17 by abekri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>

void	exit(char **arg)
{
	int	i;

	if (arg[2] != 0)
		write (1, "bash: exit: too many arguments\n", 31);
	else if (arg[1] != 0)
	{
		i = 0;
		while (arg[1][i] != 0)
		{
			if (arg[1][i] < '0' && arg[1][i] > '9')
			{
				printf("exit: %s: numeric argument required\n", arg[1]);
				break ;
			}
			i++;
		}
		//call free memory function.
		exit (arg[1]);
	}
	else
	{
		//call free memory function.
		exit (0);
	}
}
