# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gyeepach <gyeepach@student.42bangkok.com>  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/03/21 16:19:53 by jjaroens          #+#    #+#              #
#    Updated: 2026/07/02 21:12:45 by gyeepach         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv

SRC = srcs/main.cpp srcs/Server.cpp srcs/Client.cpp srcs/Command.cpp srcs/Parser.cpp srcs/Lexer.cpp \
		srcs/Channel.cpp srcs/Join.cpp srcs/Invite.cpp srcs/Kick.cpp srcs/Cap.cpp srcs/Topic.cpp \
		srcs/Help.cpp srcs/Part.cpp srcs/Mode.cpp srcs/Privmsg.cpp srcs/User.cpp srcs/Quit.cpp \
		srcs/Pass.cpp srcs/Nick.cpp

HEADER = include/Server.hpp include/Client.hpp include/Channel.hpp include/Parser.hpp

CC = c++ -Wall -Wextra -Werror -std=c++98 -g3

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