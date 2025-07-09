# ============================
# 🎨 COLOR CONFIGURATION
# ============================
RED     = \033[91;1m
GREEN   = \033[92;1m
YELLOW  = \033[93;1m
BLUE    = \033[94;1m
PINK    = \033[95;1m
CLEAR   = \033[0m

# ============================
# 🔧 PROJECT CONFIG
# ============================
NAME    := ircbot
OBJ_DIR := obj
DIR_INCLUDES := includes
CC      := c++
CFLAGS  := -Wall -Wextra -Werror -std=c++98 -I$(DIR_INCLUDES)
DEBUG	:= -g3 -fsanitize=address,leak,undefined

vpath 	%.cpp src

SRC     :=	main.cpp \
			Bot.cpp 

OBJ = $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(SRC))


# ============================
# 🧱 BUILD RULES
# ============================

all: $(NAME)

$(NAME): $(OBJ)
	@echo "$(PINK)✨ Linking executable...$(CLEAR)"
	$(CC) $(OBJ) $(CFLAGS) $(DEBUG) -o $(NAME)
	@echo "$(GREEN)✅ Build successful!$(CLEAR)"

$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	@echo "$(BLUE)📦 Compiling $< -> $@$(CLEAR)"
	$(CC) $(CFLAGS) $(DEBUG) -c $< -o $@
	@echo "$(GREEN)✔ Done.$(CLEAR)"

$(OBJ_DIR):
	mkdir -pv $@

clean:
	@echo "$(YELLOW)🧹 Cleaning object files...$(CLEAR)"
	@rm -rf $(OBJ_DIR)
	@echo "$(GREEN)✔ Object files removed.$(CLEAR)"

fclean: clean
	@echo "$(YELLOW)🧽 Removing executable...$(CLEAR)"
	@rm -rf $(NAME)
	@echo "$(GREEN)✔ Executable removed.$(CLEAR)"

re: fclean all

.PHONY: all clean fclean re test