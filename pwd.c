#include <unistd.h>
#include <stdio.h>

int	pwd(int fd)
{
	char buffer[4096];

	if (getcwd(buffer, 4096) == NULL)
		write(2, "error getting the directory\n", 28);
	else if (fd > 2)
		return (buffer);
	else
		printf("\n%s\n", buffer);
}
