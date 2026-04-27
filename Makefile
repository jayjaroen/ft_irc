# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jjaroens <jjaroens@student.42bangkok.co    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/03/21 16:19:53 by jjaroens          #+#    #+#              #
#    Updated: 2026/04/17 17:01:15 by jjaroens         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ft_irc

SRC = srcs/main.cpp srcs/Server.cpp srcs/Client.cpp command.cpp parser.cpp lexer.cpp

HEADER = include/Server.hpp include/Client.hpp

CC = c++ -Wall -Wextra -Werror -std=c++98

OBJ_DIR = obj/

OBJ = $(SRC:%.cpp=$(OBJ_DIR)%.o)

all: $(NAME)

$(NAME): $(OBJ) $(HEADER)
		$(CC) $(OBJ) -o $(NAME) 

$(OBJ_DIR)%.o: %.cpp $(HEADER) Makefile
		@mkdir -p $(dir $@)
		$(CC) -I. -o $@ -c $<

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re