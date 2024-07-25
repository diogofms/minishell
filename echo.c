#include <unistd.h>
#include "builtins.h"

void	echo(char **args, int fd)
{
	int		i;
	int		j;
	int		extra_line;
	char	*str;
	char	**env_;

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
			write (fd, pwd(3), ft_strlen(pwd(3)));
		else if (ft_strcmp(args[i], "env") == 1)
		{
			env_ = env();
			j = 0;
			while (env_[j] != NULL)
			{
				write (fd, env_[j], ft_strlen(env_[j]));
				j++;
			}
		}
		else
		{
			write (fd, args[i], ft_strlen(args[i]));
			if (args[i + 1] != 0)
				write (fd, " ", 1);
		}
		i++;
	}
	write (1, "\n", 1);
	if (extra_line == 1)
		write (fd, "\n", 1);
}
