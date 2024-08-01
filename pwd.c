/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: disilva <disilva@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 17:59:05 by disilva           #+#    #+#             */
/*   Updated: 2024/08/01 17:59:06 by disilva          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <errno.h>

void	pwd(int fd)
{
	char buffer[4096];

	if (getcwd(buffer, 4096) == NULL)
		write (fd, strerror(errno), ft_strlen(strerror(errno)));
	else
		write (fd, buffer, ft_strlen(buffer));
	write (fd, "\n", 1);
}
