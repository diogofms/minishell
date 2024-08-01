#include <unistd.h>
#include <stdio.h>
#include <errno.h>

void	pwd(int fd)
{
	char buffer[4096];

	if (getcwd(buffer, 4096) == NULL)
		write (fd, strerror(errno), ft_strlen(strerror(errno)));
	else
		write (fd, buffer, ft_strlen(buffer));
	write (fd, "\n", 1);
}
