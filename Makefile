CC = gcc
CFLAGS = -Wall -Wextra -Werror
CFLAGS_MEM = -Wall -Wextra -Werror -g3 -fsanitize=address

all: my_tar

my_tar: my_tar.o my_c_functions.o posix_header_filler.o linked_list.o archive_parser.o
	${CC} -o $@ $^ ${CFLAGS}

%.o: %.c %.h
	${CC} -c $<

clean:
	rm *.o core.* *.tar

tarclean:
	rm *.tar

fclean:
	rm -rf main header parser my_tar

cleanall: fclean clean
