#include <unistd.h>
#include "builtins.h"

void	echo(char **args, int fd)
{
	int		i;
	int		j;
	int		extra_line;
	char	*str;

	extra_line = 1;
	i = 1;
	if (args[1] == "-n")
	{
		extra_line = 0;
		i = 2;
	}
	write (1, "\n", 1);
	while (args[i] != 0)
	{
		if (ft_strcmp(args[i], "pwd") == 1)
			str = pwd(3);
		else
		{
			j = 0;
			while (args[i][j] != 0)
				j++;
			write (fd, args[i], j);
			if (args[i + 1] != 0)
				write (fd, " ", 1);
		}
		i++;
	}
	write (1, "\n", 1);
	if (extra_line == 1)
		write (fd, "\n", 1);
}
