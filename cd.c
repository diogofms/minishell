#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include "builtins.h"

int ft_cd(char **args)
{
	int             i;
	int             j;
	DIR 			*dir;
	struct dirent   *entry;
	char            cwd[4096];
	char			*str;

	// if (getcwd(cwd, 4096) == NULL)
	// {
	// 	erro = strerror(errno);
	// 	write(1, erro, ft_strlen(erro));
	// 	write(1, "\n", 1);
	// }
	i = 0;
	while (args[i] != 0)
	{
		if (ft_strcmp("..", args[i]) == 1)
		
		dir = opendir(".");
		if (dir == NULL)
			ft_print_erro(strerror(errno));
		while ((str = readdir(dir)) != NULL)
		{
			if (ft_strcmp(str, args[i]) == 1)
			{
				if ((chdir(str)) != 0)
					ft_print_erro(strerror(errno));
				break;
			}
		}
		closedir(dir);
		if (str == NULL)
		{
			printf ("bash: cd: %s: No such file or directory", args[i]);
			return (0);
		}
		i++;
	}
}

void	ft_print_erro(char *str)
{
	write(1, str, ft_strlen(str));
	write(1, "\n", 1);
}
