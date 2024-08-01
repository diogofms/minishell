#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include "builtins.h"

int	ft_strlen(char *str, char end)
{
	int	i;

	i = 0;
	while (str[i] != end)
		i++;
	return (i);
}

int	ft_strcmp(char *a, char *b)
{
	int	i;

	i = 0;
	while (a[i] == b[i])
		i++;
	if (a[i] == 0 && b[i] == 0)
		return (1);
	return (0);
}

int	malloc_fail(void *str, char *error)
{
	if (str == NULL)
	{
		printf ("%s\n", error);
		return (1);
	}
	return (0);
}

int	add_node(env_list_t **tail, int size)
{
	*tail = malloc(sizeof(env_list_t));
	if (malloc_fail(*tail,  strerror(errno)) == 1)
		return(1);
	(*tail)->content = malloc(size + 1);
	if (malloc_fail((*tail)->content,  strerror(errno)) == 1)
	{
		free (*tail);
		*tail = NULL;
		return(1);
	}
	(*tail)->next = NULL;
	return (0);
}

void	str_cpy(char *a, char **b, int end)
{
	int	i;

	i = 0;
	while (a[i] != end)
	{
		*b[i] = a[i];
		i++;
	}
	*b[i] = 0;
}

char	*strcnc(char *a, char *b)
{
	char	*new_str;
	int		i;
	int		j;

	new_str = malloc(ft_strlen(a, 0) + ft_strlen(b, 0) + 1);
	if (malloc_fail(new_str, strerror(errno)) == 1)
		return (NULL);
	i = 0;
	while (a[i] != 0)
	{
		new_str[i] = a[i];
		i++;
	}
	j = 0;
	while (b[j] != 0)
	{
		new_str[i] = b[j];
		i++;
		j++;
	}
	new_str[i] = 0;
	return (new_str);
}
