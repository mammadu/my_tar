CC = gcc
CFLAGS = -Wall -Wextra -Werror
CFLAGS_MEM = -Wall -Wextra -Werror -g3 -fsanitize=address

FILES = my_tar my_c_functions posix_header_filler linked_list archive_parser
OBJECT_LOC = obj
OBJECT_FILES = $(patsubst %, ${OBJECT_LOC}/%.o, ${FILES})
SOURCE_LOC = src
SOURCE_FILES = $(patsubst %, ${SOURCE_LOC}/%.c, ${FILES})
HEADER_LOC = include
HEADER_FILES = $(patsubst %, ${HEADER_LOC}/%.h, ${FILES})

NAME = my_tar

all: ${NAME}

${NAME}: ${OBJECT_FILES}
	${CC} -o $@ $^ ${CFLAGS}

${OBJECT_LOC}/%.o: ${SOURCE_LOC}/%.c ${HEADER_LOC}/%.h
	${CC} -I ${HEADER_LOC} -c $< -o $@

clean:
	rm ${OBJECT_LOC}/*.o core.* *.tar

tarclean:
	rm *.tar

fclean:
	rm -rf main header parser my_tar

cleanall: fclean clean
