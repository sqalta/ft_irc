NAME := ircserv

SRCS := \
	Channel/Channel.cpp \
	Client/Client.cpp \
	Server/Server.cpp \
	main.cpp \
	Commands/Nick.cpp \
	Commands/Pass.cpp \
	Commands/User.cpp \
	Commands/Join.cpp \
	Commands/Privmsg.cpp \
	Commands/Who.cpp \
	Commands/Kick.cpp \
	Commands/Part.cpp \
	Commands/Topic.cpp \
	Commands/Quit.cpp \
	Commands/Notice.cpp \
	Commands/Mode.cpp \
	Commands/ValidationUtils.cpp

OBJ_DIR := obj
OBJS := $(addprefix $(OBJ_DIR)/, $(SRCS:.cpp=.o))
CXX := c++
CXXFLAGS := -g -Wall -Wextra -Werror -std=c++98
RM := rm -rf

# Terminal colors
CYAN := \033[1;36m
RESET := \033[0m

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJS)
	@echo "$(CYAN)[Linking]$(RESET) $@"
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(OBJ_DIR)/$(dir $<)
	@echo "$(CYAN)[Compiling]$(RESET) $<"
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@echo "$(CYAN)[Cleaning]$(RESET) object files"
	$(RM) $(OBJ_DIR)

fclean:
	@echo "$(CYAN)[Cleaning]$(RESET) object files and program"
	$(RM) $(OBJ_DIR)
	$(RM) $(NAME)

re: fclean all
	@echo "$(CYAN)[Rebuilt]$(RESET) project"