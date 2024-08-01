#ifndef BUILTINS_H
# define BUILTINS_H

typedef struct  env_list_s{
char		*content;
env_list_t	*next;
}	env_list_t;

int		ft_strlen(char *str, char end);
int		ft_strcmp(char *a, char *b);
int		pwd(int fd);
int		malloc_fail(void *str, char *error);
int		add_node(env_list_t *tail, int size);
void	str_cpy(char *a, char **b, int end);
int		env_cmp(char *current_env, char *new_env);
char	*strcnc(char *a, char *b);

#endif
