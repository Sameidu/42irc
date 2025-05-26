NAME := ircserver

DIR_SRCS := srcs
DIR_INCLUDES := includes
DIR_OBJS := objs

CXX := c++
CXXFLAGS := -Wall -Wextra -Werror -std=c++98 -I$(DIR_INCLUDES) -g3 -fsanitize=address,leak,undefined

SRCS :=	$(DIR_SRCS)/main.cpp \
		$(DIR_SRCS)/Server.cpp \
		$(DIR_SRCS)/Client.cpp \

OBJS := $(patsubst $(DIR_SRCS)/%.cpp, $(DIR_OBJS)/%.o, $(SRCS))

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(DIR_OBJS)/%.o: $(DIR_SRCS)/%.cpp
	@mkdir -p $(DIR_OBJS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(DIR_OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re