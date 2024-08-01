#include <stdio.h>
#include <unistd.h>
#include "builtins.h"

int	env(env_list_t *head, int fd)
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
