################################################################################
#                                    CONFIG                                    #
################################################################################

NAME		 	= ft_containers_tests
NAME_STD	 	= ft_containers_tests_std
NAME_TREE	 	= _tree_tests
NAME_HACK_STACK	= demo_hack_stack
NAME_42		 	= 42_tests
NAME_42_STD	 	= 42_tests_std
AUTHOR		 	= jmazoyer

CC			= clang++
CFLAGS		= -Wall -Wextra -Werror -std=c++98
CPPFLAGS	= $(foreach path, $(INC_PATH), -I$(path))
CPPFLAGS	+= -MMD
RM			= rm -fr

V			= 0
N			= 0
S			= 0
F			= 1

################################################################################
#                               SRCS - INCLUDES                                #
################################################################################

INC_PATH		= includes			\
				  tests/includes

SRC_PATH		= tests				\
				  tests/srcs

SRC_NAME		=	main.cpp							\
					enable_if__is_integral__tests.cpp	\
					equal__tests.cpp					\
					lexicographical_compare__tests.cpp	\
					pair__tests.cpp						\
					make_pair__tests.cpp				\
					iterator_traits__tests.cpp			\
					reverse_iterator__tests.cpp			\
					vector__tests.cpp					\
					stack__tests.cpp					\
					map__tests.cpp						\
					set__tests.cpp

SRC_TREE		=	_tree__tests.cpp

SRC_HACK_STACK	=	hack_stack_main.cpp

SRC_42			=	42_main.cpp

OBJ_PATH		= objs
OBJ				= $(addprefix $(OBJ_PATH)/, $(SRC_NAME:.cpp=.o))
OBJ_STD			= $(addprefix $(OBJ_PATH)/, $(SRC_NAME:.cpp=_std.o))

OBJ_TREE		= $(addprefix $(OBJ_PATH)/, $(SRC_TREE:.cpp=.o))

OBJ_HACK_STACK	= $(addprefix $(OBJ_PATH)/, $(SRC_HACK_STACK:.cpp=.o))

OBJ_42			= $(addprefix $(OBJ_PATH)/, $(SRC_42:.cpp=.o))
OBJ_42_STD		= $(addprefix $(OBJ_PATH)/, $(SRC_42:.cpp=_std.o))

DEP				= $(OBJ:.o=.d) $(OBJ_STD:.o=.d) $(OBJ_TREE:.o=.d) \
					$(OBJ_42:.o=.d)

VPATH			= $(SRC_PATH)

################################################################################
#                          COLORS - STRINGS - SYMBOLS                          #
################################################################################

RED		  = \033[0;31m
B_RED	  = \033[0;91m
GREEN	  = \033[0;32m
B_GREEN	  = \033[0;92m
YELLOW	  = \033[0;33m
B_YELLOW  = \033[0;93m
BLUE	  = \033[0;34m
B_BLUE	  = \033[0;94m
MAGENTA	  = \033[0;35m
B_MAGENTA = \033[0;95m
CYAN	  = \033[0;36m
B_CYAN	  = \033[0;96m
GRAY	  = \033[0;90m
B_GRAY	  = \033[0;37m
NO_COLOR  = \033[m

MKDIR	  = Creating directory
COMPIL	  = Compiling & assembling
FLAGS	  = Compiler flags
LINK	  = Linking

################################################################################
#                                  FUNCTIONS                                   #
################################################################################

ifeq ($(OS),Windows_NT) 
    detected_OS := Windows
else
    detected_OS := $(shell sh -c 'uname 2>/dev/null || echo Unknown')
endif

ifeq ($(detected_OS),Darwin) 
	RUN_CMD = script -q "$(@F).log" $1 > /dev/null; \
				RESULT=$$?
else ifeq ($(detected_OS),Linux)
	RUN_CMD = script -q -e -c "$(1)" "$(@F).log" > /dev/null; \
				RESULT=$$?; \
				sed -i '1d' "$(@F).log"; \
				sed -i "$$(($$(wc -l < "$(@F).log")-1)),\$$d" "$(@F).log"
else
	RUN_CMD = $(1) 2> "$(@F).log"; \
				RESULT=$$?
endif

define run
if [ $(S) -eq 1 ]; then \
	:; \
elif [ $(N) -eq 1 ]; then \
	printf "%b\n" "$(1)"; \
elif [ $(V) -eq 1 ]; then \
	printf "%b\n" "$(GRAY)$(1)"; \
elif ( echo "$(@F)" | grep -q '\.o' ) && [ $(F) -eq 1 ]; then \
	printf "%-30b%-35b\n" "$(BLUE)$(FLAGS)" "$(GRAY)$(CFLAGS)$(NO_COLOR)"; \
fi
if [ $(S) -eq 1 ]; then \
	:; \
elif ( echo "$(2)" | grep -q 'clean' ) && [ $(N) -eq 0 ]; then \
	printf "%-58b" "$(BLUE)$(2)$(NO_COLOR)"; \
elif [ $(N) -eq 0 ]; then \
	printf "%-30b%-35b" "$(BLUE)$(2)" "$(3)$(@F)$(NO_COLOR)"; \
fi
if ( echo "$(2)" | grep -q 'clean' ); then \
	$(1); \
	RESULT=$$?; \
else \
	$(RUN_CMD); \
fi ; \
if [ $(S) -eq 1 ]; then \
	:; \
elif [ $(N) -eq 0 -a $$RESULT -ne 0 ]; then \
	printf "%b\n" "$(RED)[KO]"; \
elif [ $(N) -eq 0 -a -s "$(@F).log" ]; then \
	printf "%b\n" "$(YELLOW)[WARN]"; \
elif [ $(N) -eq 0 ]; then  \
	printf "%b\n" "$(GREEN)[OK]"; \
fi; \
printf "%b" "$(NO_COLOR)"; \
cat "$(@F).log" 2> /dev/null; \
rm -f "$(@F).log"; \
exit $$RESULT
endef

################################################################################
#                                    RULES                                     #
################################################################################

all: header $(NAME) $(NAME_STD)

debug: header add_flags $(NAME) $(NAME_STD)
	$(eval CFLAGS = -Wall -Wextra -Werror -std=c++98)

add_flags:
	$(eval CFLAGS += -g -fsanitize=address,undefined -fno-omit-frame-pointer)

tree: header $(NAME_TREE)

demo: header $(NAME_HACK_STACK)

test: all tester_42 demo
	@./tests/run_tests.sh

tester_42: header $(NAME_42) $(NAME_42_STD)

header:
	@if [ $(S) -eq 1 ]; then \
		:; \
	else \
		printf "%b" "$(GREEN)"; \
		echo "           ___  _____ ___  ___      _        "; \
		echo "          /   |/ __  \|  \/  |     | |       "; \
		echo "         / /| |\`' / /'| .  . | __ _| | _____ "; \
		echo "        / /_| |  / /  | |\/| |/ _\` | |/ / _ \\"; \
		echo "        \___  |./ /___| |  | | (_| |   <  __/"; \
		echo "            |_/\_____/\_|  |_/\__,_|_|\_\___|"; \
		printf "%43b" "(run with \"V=1\" for Verbose)\n"; \
		printf "%38b" "(\"N=1\" for Normal)\n"; \
		printf "%42b" "(\"S=1\" for Silent)$(NO_COLOR)\n\n"; \
		printf "%b" "$(BLUE)Name:	$(GREEN)$(NAME)\n"; \
		printf "%b" "$(BLUE)Author:	$(GREEN)$(AUTHOR)$(NO_COLOR)\n\n"; \
	fi

$(NAME): $(OBJ)
	@$(call run,$(CC) $(CFLAGS) $(OBJ) -o $@,$(LINK),$(B_GREEN))
	$(eval F=1)

$(NAME_STD): $(OBJ_STD)
	@$(call run,$(CC) $(CFLAGS) $(OBJ_STD) -o $@,$(LINK),$(B_GREEN))
	$(eval F=1)

$(NAME_TREE): $(OBJ_TREE)
	@$(call run,$(CC) $(CFLAGS) $(OBJ_TREE) -o $@,$(LINK),$(B_GREEN))
	$(eval F=1)

$(NAME_HACK_STACK): $(OBJ_HACK_STACK)
	@$(call run,$(CC) $(CFLAGS) $(OBJ_HACK_STACK) -o $@,$(LINK),$(B_GREEN))
	$(eval F=1)

$(NAME_42): $(OBJ_42)
	@$(call run,$(CC) $(CFLAGS) $(OBJ_42) -o $@,$(LINK),$(B_GREEN))
	$(eval F=1)

$(NAME_42_STD): $(OBJ_42_STD)
	@$(call run,$(CC) $(CFLAGS) $(OBJ_42_STD) -o $@,$(LINK),$(B_GREEN))
	$(eval F=1)

$(OBJ_PATH)/%.o: %.cpp | $(OBJ_PATH)
	@$(call run,$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@,$(COMPIL),$(B_CYAN))
	$(eval F=0)

$(OBJ_PATH)/%_std.o: %.cpp | $(OBJ_PATH)
	@$(call run,$(CC) $(CFLAGS) $(CPPFLAGS) -D STD -c $< -o $@\
		,$(COMPIL),$(B_CYAN))
	$(eval F=0)

$(OBJ_PATH):
	@$(call run,mkdir -p $@,$(MKDIR),$(B_BLUE))

clean: header
	@$(call run,$(RM) $(OBJ_PATH) *.log,clean)

fclean: clean
	@$(call run,$(RM) $(NAME) $(NAME_STD) $(NAME_TREE) $(NAME_HACK_STACK) \
		$(NAME_42) $(NAME_42_STD),fclean)

re: fclean all

-include $(DEP)

.PHONY: all debug add_flags tree demo test tester_42 header clean fclean re
