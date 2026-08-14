SOURCES		= main.c start.c initalize_server.c socket.c bind.c listen.c run.c client.c client_utils.c message.c broadcast.c signals.c release_ressources.c
HEADERS		= error.h server.h client.h signals.h
OBJECTS_DIR = .objects/
DEPS_DIR	= .deps/
OBJECTS     = ${SOURCES:%.c=${OBJECTS_DIR}%.o}
DEPS		= ${SOURCES:%.c=${DEPS_DIR}%.d}
NAME		= seddari
CC         = cc
CC_FLAGS   = -Wall -Wextra -Werror
DEPS_FLAGS	= -MT $@ -MMD -MP -MF $(DEPS_DIR)$*.d

all: ${NAME}
	@echo "Building done: ${NAME} is ready."

${OBJECTS_DIR}%.o : %.c | ${DEPS_DIR} ${OBJECTS_DIR}
	@echo "Compiling $< ..."
	@${CC} ${CC_FLAGS} ${DEPS_FLAGS} -c $< -o $@

${NAME}: ${OBJECTS} ${OBJECTS_DIR} ${DEPS_DIR} ${HEADERS}
	@${CC} ${CC_FLAGS} ${OBJECTS} -o ${NAME}
	@echo "${NAME} has been successfully generated."

${OBJECTS_DIR} ${DEPS_DIR}:
	@echo "Creating $@ folder..."
	@mkdir -p $@

clean:
	@echo "Deleting objects and dependency files..."
	@rm -rf ${OBJECTS_DIR} ${DEPS_DIR}
	@echo "Objects and dependency files deleted."

fclean: clean
	@echo "Deleting executables, objects and dependency files..."
	@rm -f ${NAME}
	@echo "Executables, objects and dependency files deleted."
re: fclean all
	@echo "🔄"

-include ${DEPS}

.PHONY: all clean fclean re
