CC = gcc
CFLAGS = -Wall -std=c99 -O3
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

healkristin: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c $(wildcard *.h)
	$(CC) $(CFLAGS) -c $< -o $@

debug: CFLAGS = -Wall -std=c99 -g
debug: clean healkristin

clean:
	rm -f healkristin *.o

.PHONY: clean debug
