SOURCES_DIR     := sources/
SOURCES         := \
                   $(SOURCES_DIR)main.c \
                   $(SOURCES_DIR)options.c \
                   $(SOURCES_DIR)start.c \
                   $(SOURCES_DIR)initialize_server.c \
                   $(SOURCES_DIR)socket.c \
                   $(SOURCES_DIR)bind.c \
                   $(SOURCES_DIR)listen.c \
                   $(SOURCES_DIR)run.c \
                   $(SOURCES_DIR)client.c \
                   $(SOURCES_DIR)client_utils.c \
                   ${SOURCES_DIR}client_commands.c \
                   ${SOURCES_DIR}recv_utils.c \
                   ${SOURCES_DIR}send_utils.c \
                   $(SOURCES_DIR)message.c \
                   $(SOURCES_DIR)time.c \
                   $(SOURCES_DIR)broadcast.c \
                   $(SOURCES_DIR)signals.c \
                   ${SOURCES_DIR}metrics.c   \
                   ${SOURCES_DIR}utils.c    \
                   $(SOURCES_DIR)release_resources.c

HEADERS_DIR     := includes/
HEADERS         := \
                   $(HEADERS_DIR)status.h \
                   $(HEADERS_DIR)server.h \
                   $(HEADERS_DIR)client.h \
                   $(HEADERS_DIR)signals.h

OBJECTS_DIR     := .objects/
DEPS_DIR        := .deps/

OBJECTS         := $(patsubst $(SOURCES_DIR)%.c,$(OBJECTS_DIR)%.o,$(SOURCES))
DEPS            := $(patsubst $(SOURCES_DIR)%.c,$(DEPS_DIR)%.d,$(SOURCES))

NAME            := 	seddari
CC              := 	cc
CC_FLAGS        := 	-Wall -Wextra -Werror -I$(HEADERS_DIR)
FSAN_ADDR_FLAG	:=	-fsanitize=address

.PHONY: all clean fclean re

all: $(NAME)
	@echo "Building done: $(NAME) is ready."

$(OBJECTS_DIR)%.o: $(SOURCES_DIR)%.c | $(OBJECTS_DIR) $(DEPS_DIR)
	@echo "Compiling $< ..."
	@$(CC) $(CC_FLAGS) -MMD -MP -MF $(DEPS_DIR)$*.d -MT $@ -c $< -o $@

$(NAME): $(OBJECTS) $(HEADERS)
	@$(CC) $(CC_FLAGS) $(OBJECTS) -o $(NAME)
	@echo "$(NAME) has been successfully generated."

fsan_addr: $(OBJECTS) $(HEADERS)
	@$(CC) $(CC_FLAGS) ${FSAN_ADDR_FLAG} $(OBJECTS) -o $(NAME)
	@echo "$(NAME) has been successfully generated."

$(OBJECTS_DIR) $(DEPS_DIR):
	@echo "Creating $@ folder..."
	@mkdir -p $@

clean:
	@echo "Deleting objects and dependency files..."
	@rm -rf $(OBJECTS_DIR) $(DEPS_DIR)
	@echo "Objects and dependency files deleted."

fclean: clean
	@echo "Deleting executables, objects and dependency files..."
	@rm -f $(NAME)
	@echo "Executables, objects and dependency files deleted."

re: fclean all
	@echo "🔄"

-include $(DEPS)
