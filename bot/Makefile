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

vpath 	%.cpp src src/cmds src/botCmds

SRC     :=	main.cpp \
			Bot.cpp \
			CmInvite.cpp \
			CmPrivmsg.cpp \
			CmJoin.cpp \
			Help.cpp \
			Dice.cpp \
			Wisdom.cpp \
			MagicBall.cpp \
			Nones.cpp \
			RPS.cpp \
			SaySomething.cpp \

OBJ = $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(SRC))

all: $(NAME)

$(NAME): $(OBJ)
	@echo -e "$(PINK)✨ Linking executable...$(CLEAR)"
	$(CC) $(OBJ) $(CFLAGS) $(DEBUG) -o $(NAME)
	@echo -e "$(GREEN)✅ Build successful!$(CLEAR)"

$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	@echo -e "$(BLUE)📦 Compiling $< -> $@$(CLEAR)"
	$(CC) $(CFLAGS) $(DEBUG) -c $< -o $@
	@echo -e "$(GREEN)✔ Done.$(CLEAR)"

$(OBJ_DIR):
	mkdir -pv $@

clean:
	@echo -e "$(YELLOW)🧹 Cleaning object files...$(CLEAR)"
	@rm -rf $(OBJ_DIR)
	@echo -e "$(GREEN)✔ Object files removed.$(CLEAR)"

fclean: clean
	@echo -e "$(YELLOW)🧽 Removing executable...$(CLEAR)"
	@rm -rf $(NAME)
	@echo -e "$(GREEN)✔ Executable removed.$(CLEAR)"

re: fclean all

.PHONY: all clean fclean re