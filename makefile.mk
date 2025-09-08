NAME	= philo

DIR		= .
SRC_DIR	= $(DIR)/srcs
INC_DIR = $(DIR)/includes

SRCS = main.c check_input.c init.c threads.c utils.c getters.c actions.c monitor.c logging.c mutex_utils.c scribe.c sim_utils.c
SRCS := $(addprefix $(SRC_DIR)/, $(SRCS))

vpath %.c $(SRC_DIR)

vpath %.h $(INC_DIR)