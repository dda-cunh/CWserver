.SILENT:
NAME		= 	cwserver

CC 			= 	cc

CFLAGS		= 	-Wall -Wextra -Werror -gdwarf-4 #-fsanitize=address

RM 			= 	rm -rf

INC_DIR		=	inc/

SRC_DIR		=	src/

HTTP_DIR	=	http/

OBJ_DIR		=	temp/

TYPES_DIR	=	t_types/

UTILS_DIR	= 	utils/

SRC			=	$(addprefix $(SRC_DIR),	main.c \
										$(addprefix $(UTILS_DIR),	ut_memory.c \
																	ut_num.c \
																	ut_output.c \
																	ut_string.c \
																	ut_generic.c) \
										$(addprefix $(TYPES_DIR),	t_response.c \
																	t_server.c \
																	t_request.c \
																	t_byte_array.c) \
										$(addprefix $(HTTP_DIR),	methods.c))

OBJ_DIRS	=	$(OBJ_DIR)	$(addprefix $(OBJ_DIR), $(TYPES_DIR)) \
							$(addprefix $(OBJ_DIR), $(UTILS_DIR)) \
							$(addprefix $(OBJ_DIR), $(HTTP_DIR))

OBJ 		= 	$(SRC:$(SRC_DIR)%.c=$(OBJ_DIR)%.o)

GREEN		= 	\033[0;32m

RED			=	\033[0;31m

RESET		=	\033[0m

SUS			=	\U00000D9E

HAMMER		=	\U0001F528

BROOM		=	\U0001F9F9

$(NAME):		$(OBJ) | $(SRC)
				printf '$(HAMMER)\t$(GREEN)Compiling $(NAME)$(RESET)\n'
				$(CC) $(CFLAGS) $^ -o $@ -I $(INC_DIR) $(LINKS)
				make compiled

$(OBJ_DIR)%.o:	$(SRC_DIR)%.c | $(OBJ_DIRS)
				printf '$(HAMMER)\t'
				printf "$(GREEN)Compiling $(NAME) objects... $(RED)%-33.33s\r" $(notdir $@)
				$(CC) $(CFLAGS) -c $< -o $@ -I $(INC_DIR)

$(OBJ_DIRS):
				mkdir -p $@

all: 			$(NAME)

clean:
				if [ -d $(OBJ_DIR) ]; then $(RM) $(OBJ_DIR); fi

fclean:			clean
				printf '$(BROOM)\n$(BROOM)\t$(GREEN)Cleaning project$(RESET)\n'
				$(RM) $(NAME)
				printf '$(BROOM)\t\t\t$(SUS)\n'

re:				fclean all

compiled:
				printf "															 	\n"
				printf "$(GREEN)	$(NAME)							 			$(RESET)\n"
				printf "$(GREEN)                             _  _             _ $(RESET)\n"
				printf "$(GREEN)                            (_)| |           | |$(RESET)\n"
				printf "$(GREEN)  ____   ___   ____   ____   _ | |  ____   _ | |$(RESET)\n"
				printf "$(GREEN) / ___) / _ \ |    \ |  _ \ | || | / _  ) / || |$(RESET)\n"
				printf "$(GREEN)( (___ | |_| || | | || | | || || |( (/ / ( (_| |$(RESET)\n"
				printf "$(GREEN) \____) \___/ |_|_|_|| ||_/ |_||_| \____) \____|$(RESET)\n"
				printf "$(GREEN)                     |_|                        $(RESET)\n"
				printf "																\n"

run:			all
				./$(NAME)

rerun:			fclean | run

.PHONY: 		all clean fclean re run rerun
