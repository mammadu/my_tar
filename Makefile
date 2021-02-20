CC = gcc
CFLAGS = -Wall -Wextra -Werror
CFLAGS_MEM = -Wall -Wextra -Werror -g3 -fsanitize=address

all: main

main: my_tar.o
	${CC} -o $@ $^ ${CFLAGS}

%.o: %.c %.h
	${CC} -c $<

clean:
	rm *.o core.main.*