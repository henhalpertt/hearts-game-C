# makefile sample

TARGET= t.out
CC= gcc
CFLAGS= -ansi -g -pedantic
OBJS= card.o deck.o player.o game.o round.o main.o

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c
	
card.o: card.c card.h
	$(CC) $(CFLAGS) -c card.c
	
deck.o: deck.c deck.h
	$(CC) $(CFLAGS) -c deck.c
	
player.o: player.c player.h
	$(CC) $(CFLAGS) -c player.c
	
game.o: game.c game.h
	$(CC) $(CFLAGS) -c game.c
	
round.o: round.c round.h player.h
	$(CC) $(CFLAGS) -c round.c
	
#ui.o: ui.c ui.h
#	$(CC) $(CFLAGS) -c ui.c
	
clean:
	rm -f $(TARGET) $(OBJS)
