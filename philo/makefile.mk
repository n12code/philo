NAME	= philo

DIR		= .
SRC_DIR	= $(DIR)/srcs
INC_DIR = $(DIR)/includes

SRCS = main.c forks.c check_input.c init.c threads.c getters.c actions.c monitor.c logging.c scribe.c utils_1.c utils_2.c utils_3.c
SRCS := $(addprefix $(SRC_DIR)/, $(SRCS))

vpath %.c $(SRC_DIR)

vpath %.h $(INC_DIR)