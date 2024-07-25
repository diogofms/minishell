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
	int				k;
	DIR 			*dir;
	struct dirent   *entry;
	char            cwd[4096];
	char			*str;
	char			ogdir[4096];

	if (getcwd(ogdir, 4096) == NULL)
		ft_print_erro(strerror(errno));
	i = 0;
	if (args[0] == "/")
	{
		if ((chdir("/")) != 0)
			ft_print_erro(strerror(errno));
	}
	if (args[0] == "-" && args[1] == 0)
	{
		str = getenv("OLDPWD");
		if ((chdir(str)) != 0)
			ft_print_erro(strerror(errno));
	}
	else if (args[0] == "~")
	{
		str = getenv("HOME");
		if ((chdir(str)) != 0)
			ft_print_erro(strerror(errno));
		i++;
	}
	while (args[i] != 0)
	{
		if (args[i] == "..")
		{
			if (getcwd(cwd, 4096) == NULL)
				ft_print_erro(strerror(errno));
			j = 0;
			while (cwd[j] != 0)
				j++;
			while (cwd[j] != '/')
				j--;
			k = 0;
			while (k != j)
			{
				str[k] = cwd[k];
				k++;
			}
			if ((chdir(str)) != 0)
				ft_print_erro(strerror(errno));
		}
		else
		{
			dir = opendir(".");
			if (dir == NULL)
				ft_print_erro(strerror(errno));
			while ((entry = readdir(dir)) != NULL)
			{
				if (ft_strcmp(entry->d_name, args[i]) == 1)
				{
					if ((chdir(entry->d_name)) != 0)
						ft_print_erro(strerror(errno));
					break;
				}
			}
			closedir(dir);
			if (str == NULL)
			{
				if ((chdir(ogdir)) != 0)
						ft_print_erro(strerror(errno));
				printf ("bash: cd: ");
				i = 0;
				while (args[i] != 0)
				{
					printf ("%s", args[i]);
					if (args[i + 1] != 0)
						printf ("/");
				}
				printf (": No such file or directory\n");
				//free memory;
				exit (0);
			}
		}
		i++;
	}
}

void	ft_print_erro(char *str)
{
	write(1, str, ft_strlen(str));
	write(1, "\n", 1);
	//free memory;
	exit (0);
}
