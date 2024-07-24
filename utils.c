int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i] != 0)
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
