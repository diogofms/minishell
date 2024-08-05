/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abekri <abekri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 22:45:15 by abekri            #+#    #+#             */
/*   Updated: 2024/07/19 01:58:27 by abekri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/signals.h"

void	signal_handler(int signo)
{
	if (signo == SIGINT)
	{
		write(1, "\nminishell$ ", 12);
	}
}

void	setup_signal_handlers(void)
{
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
}
