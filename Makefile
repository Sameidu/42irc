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
NAME    := ircserv
OBJ_DIR := obj
DIR_INCLUDES := includes
CC      := c++
CFLAGS  := -Wall -Wextra -Werror -std=c++98 -I$(DIR_INCLUDES) -g3 -fsanitize=address,leak,undefined

VPATH := src src/cmds

SRC     :=	main.cpp \
			Client.cpp \
			Server.cpp \
			Channel.cpp \
			utils.cpp \
			ServerCommands.cpp \
			CmCAP.cpp \
			CmInvite.cpp \
			CmJoin.cpp \
			CmKick.cpp \
			CmList.cpp \
			CmMode.cpp \
			CmNames.cpp \
			CmNick.cpp \
			CmPart.cpp \
			CmPass.cpp \
			CmTopic.cpp \
			CmUser.cpp \
			CmPrivmsg.cpp \
			CmWho.cpp \
			CmQuit.cpp \
			CmNotice.cpp \


OBJ     := $(SRC:%.cpp=$(OBJ_DIR)/%.o)

DEPS := includes/Client.hpp \
       includes/Server.hpp \
       includes/irc.hpp \
	   includes/Channel.hpp

# ============================
# 🧱 BUILD RULES
# ============================

all: $(NAME)

# Executable build
$(NAME): $(OBJ)
	@echo -e "$(PINK)✨ Linking executable...$(CLEAR)"
	$(CC) $(OBJ) $(CFLAGS) -o $(NAME)
	@echo -e "$(GREEN)✅ Build successful!$(CLEAR)"

# Compiles each .cpp file into obj/.o
$(OBJ_DIR)/%.o: %.cpp $(DEPS)
	@mkdir -p $(OBJ_DIR)
	@echo -e "$(BLUE)📦 Compiling $< -> $@$(CLEAR)"
	$(CC) $(CFLAGS) -c $< -o $@
	@echo -e "$(GREEN)✔ Done.$(CLEAR)"

# ============================
# 🧼 CLEANING RULES
# ============================

clean:
	@echo -e "$(YELLOW)🧹 Cleaning object files...$(CLEAR)"
	@rm -rf $(OBJ_DIR)
	@echo -e "$(GREEN)✔ Object files removed.$(CLEAR)"

fclean: clean
	@echo -e "$(YELLOW)🧽 Removing executable...$(CLEAR)"
	@rm -rf $(NAME)
	@make fclean -C bot
	@echo -e "$(GREEN)✔ Executable removed.$(CLEAR)"

re: fclean all

# ============================
# 🤖 BONUS RULES
# ============================

bonus: all
	@make -C bot
	@echo -e "$(BLUE)🤖 Bonus bot compiled!$(CLEAR)"

# ============================
# 🚀 QUICK TEST RULE
# ============================
PORT     ?= 6667
PASS     ?= password

test: re bonus
	@echo -e "$(BLUE)🚀 Running server on port $(PORT) with password '$(PASS)'...$(CLEAR)"
	@./$(NAME) $(PORT) $(PASS)

.PHONY: all clean fclean re test bonus