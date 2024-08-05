# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abekri <abekri@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/11 22:46:03 by abekri            #+#    #+#              #
#    Updated: 2024/08/04 04:12:11 by abekri           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = gcc
CFLAGS = -Wall -Wextra -Werror
LIBFT = libft/libft.a
INCLUDE = -I include -I libft

SRCS = src/main.c \
       src/signals.c \
       src/history.c \
       src/terminal.c \
       src/parser/lexer.c \
       src/parser/check_syntax.c \
       src/parser/command_handler.c \
       src/parser/tokenization.c \
       src/parser/expansion.c \
       src/start_info/adjust_shell_lvl.c\
       src/start_info/format_term.c \

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT) $(INCLUDE) -lreadline

$(LIBFT):
	make -C libft

clean:
	make -C libft clean
	rm -f $(OBJS)

fclean: clean
	make -C libft fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
