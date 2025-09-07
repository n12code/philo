NAME	= philo

DIR		= .
SRC_DIR	= $(DIR)/srcs
INC_DIR = $(DIR)/includes

SRCS = main.c check_input.c init_2.c threads_2.c utils_2.c getters_2.c actions_2.c monitor_2.c logging.c mutex_utils.c scribe.c sim_utils.c
SRCS := $(addprefix $(SRC_DIR)/, $(SRCS))

vpath %.c $(SRC_DIR)

vpath %.h $(INC_DIR)