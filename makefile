CC = gcc
CFLAGS = -Wall -Werror -g
SOURCES = game.c

blackjack: game.c
	$(CC) $(SOURCES) -o blackjack $(CFLAGS)

clean:
	rm -f *.o blackjack
