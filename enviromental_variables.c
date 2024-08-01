#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include "builtins.h"

void ft_free_list(env_list_t *head)
{
	env_list_t *next;

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

env_list_t	*ft_envp(char **envp, env_list_t *head)
{
	env_list_t	*current;
	int			j;
	int			k;

	head = malloc(sizeof(env_list_t));
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
			current->next = malloc(sizeof(env_list_t));
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
