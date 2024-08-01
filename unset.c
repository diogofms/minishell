#include "builtins.h"
#include <unistd.h>

env_list_t	*unset(char **args, env_list_t *head)
{
	int			i;
	int			j;
	env_list_t	*prev;
	env_list_t	*node;

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
					break;
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
