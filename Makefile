CC = gcc
CFLAGS = -Wall -Wextra
 
healkristin: main.o handler.o
	$(CC) $(CFLAGS) -o $@ $^
 
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
 
clean:
	rm -f healkristin *.o
 
.PHONY: clean
