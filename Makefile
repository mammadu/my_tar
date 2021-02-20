CC = gcc
CFLAGS = -Wall -Wextra -Werror
CFLAGS_MEM = -Wall -Wextra -Werror -g3 -fsanitize=address

all: main

main: my_tar.o my_c_functions.o
	${CC} -o $@ $^ ${CFLAGS_MEM}

%.o: %.c %.h
	${CC} -c $<

clean:
	rm *.o core.main.*