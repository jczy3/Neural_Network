CC = gcc
CFLAGS = -Wall -Wextra -pthread
LDFLAGS = -lm -lrt

SRC = $(wildcard matrix/*.c neural/*.c util/*.c *.c)
OBJ = $(SRC:.c=.o)

all: main

main: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o main $(LDFLAGS)

clean:
	rm -f matrix/*.o neural/*.o util/*.o *.o main

run: main
	./main
