CC = gcc
CFLAGS = -Wall -Wextra -Werror
CFLAGS_MEM = -Wall -Wextra -Werror -g3 -fsanitize=address

all: my_tar

my_tar: my_tar.o my_c_functions.o posix_header_filler.o linked_list.o archive_parser.o
	${CC} -o $@ $^ ${CFLAGS_MEM}

# header: posix_header_filler.o my_c_functions.o
# 	${CC} -o $@ $^ ${CFLAGS_MEM}

# parser: archive_parser.o my_c_functions.o posix_header_filler.o linked_list.o
# 	${CC} -o $@ $^ ${CFLAGS_MEM}

# test: main parser
# 	./main -cf archive.tar dir0 link a b c 
# 	./parser -xf archive.tar

%.o: %.c %.h
	${CC} -c $<

# files:
# 	touch a b c
# 	mkdir dir0
# 	ln -s Makefile newlink

clean:
	rm *.o core.* *.tar

tarclean:
	rm *.tar

fclean:
	rm -rf main header parser my_tar

cleanall: fclean clean