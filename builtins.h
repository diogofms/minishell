/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: disilva <disilva@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 17:56:41 by disilva           #+#    #+#             */
/*   Updated: 2024/08/01 17:58:15 by disilva          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

typedef struct env_list_s{
	char		*content;
	t_env_list	*next;
}	t_env_list;

int		ft_strlen(char *str, char end);
int		ft_strcmp(char *a, char *b);
int		pwd(int fd);
int		malloc_fail(void *str, char *error);
int		add_node(t_env_list *tail, int size);
void	str_cpy(char *a, char **b, int end);
int		env_cmp(char *current_env, char *new_env);
char	*strcnc(char *a, char *b);

#endif
