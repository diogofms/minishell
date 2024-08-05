/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abekri <abekri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 22:46:27 by abekri            #+#    #+#             */
/*   Updated: 2024/08/04 04:32:00 by abekri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/signals.h"

volatile sig_atomic_t	g_signal = 0;

int	main(void)
{
	char			**env_vars;
	char			terminal_name[MAX_STRING_LEN];
	char			**env_copy[MAX_ENV_VARS];
	int				exit_cd;
	t_shell_config	config;

	terminal_name[BUFFER_SIZE] = "minishell$ ";
	init_shell();
	start_info_ms(env_vars, &terminal_name, *env_copy, &exit_cd);
	setup_signal_handlers();
	start_iterm(&config);
	return (0);
}
