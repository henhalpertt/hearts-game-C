# makefile sample

TARGET= t.out
CC= gcc
CFLAGS= -ansi -pedantic
OBJS= card.o deck.o player.o game.o round.o main.o UI.o

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
	
UI.o: UI.c UI.h
	$(CC) $(CFLAGS) -c UI.c
	
clean:
	rm -f $(TARGET) $(OBJS)
