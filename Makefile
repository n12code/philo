# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nbodin <nbodin@student.42lyon.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/28 11:10:39 by nbodin            #+#    #+#              #
#    Updated: 2025/08/30 14:11:38 by nbodin           ###   ########lyon.fr    #
#                                                                              #
# **************************************************************************** #

BINARIES	= philo

ifeq ($(filter bonus clean fclean,$(MAKECMDGOALS)),)
	include makefile.mk
endif

OBJ_DIR = .obj
DEP_DIR = .dep

CC		= cc
CFLAGS	= -g3 -Wall -Wextra -Werror
D_FLAGS = -MMD -MP -MF $(DEP_DIR)/$*.d
I_FLAGS = -I$(INC_DIR)
CF		= $(CC) $(CFLAGS) $(I_FLAGS)

vpath %.o $(OBJ_DIR)
vpath %.d $(DEP_DIR)

OBJS	= $(addprefix $(OBJ_DIR)/, $(notdir $(SRCS:.c=.o)))
DEPS	= $(addprefix $(DEP_DIR)/, $(notdir $(SRCS:.c=.d)))

all: $(NAME)

$(NAME): $(OBJS)
	@echo "creating $(NAME)\n"
	@$(CF) $^ -o $@
	@echo "program created\n"

bonus: $(NAME)

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR) $(DEP_DIR)
	@echo "compilation $< $@"
	@$(CF) $(D_FLAGS) -c $< -o $@

$(OBJ_DIR):
	@echo "creating obj directory"
	@mkdir -p $@

$(DEP_DIR):
	@echo "creating dep directory"
	@mkdir -p $@

clean:
	@echo "deleting obj/dep files"
	@rm -rf $(OBJ_DIR) $(DEP_DIR)

fclean:
	@echo "deleting obj/dep files"
	@rm -rf $(OBJ_DIR) $(DEP_DIR)
	@echo "removing binaries"
	@rm -f $(BINARIES)

re: fclean all

val: all
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(NAME) 

-include $(DEPS)

.PHONY: all clean fclean re debug help

FORCE: