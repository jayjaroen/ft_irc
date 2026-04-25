NAME = ircserv
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -g3
CXX = c++

SRCS = main.cpp command.cpp parser.cpp lexer.cpp ./srcs/Client.cpp ./srcs/Server.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME) : $(OBJS)
	@$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJS)

fclean: clean
	@rm -rf $(NAME)

re: clean all
.PHONY: all clean fclean re