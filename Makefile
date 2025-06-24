# Name of the binary
NAME     =	ft_nm

# Source and include directories
DIR      =	src/
INC_DIR  =	inc/

SRC	     =	src/main.c \
            src/library.c \
            src/sort.c \
            src/parser.c \
			src/clean.c


OBJ_DIR	 =	obj/

# Compiler and flags
CC       =	cc
CFLAGS   =	-Wall -Wextra -Werror -g3 -I$(INC_DIR)
LDFLAGS  =	-lelf

# Source to object mapping
OBJ      =	$(patsubst src/%.c, $(OBJ_DIR)%.o, $(SRC))

# Rules
all: $(NAME)

# Compile source files into object files
$(OBJ_DIR)%.o: src/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

# Link final binary
$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) -o $@ $(LDFLAGS)

clean:
	@rm -rf $(OBJ_DIR)
	@echo "\033[1;31m======== object files removed ========\033[0m"

fclean: clean
	@rm -f $(NAME)
	@echo "\033[1;31m========  executable removed  =======\033[0m"

re: fclean all

.PHONY: clean fclean all re
