# makefile sample

TARGET= t.out
CC= gcc
CFLAGS= -ansi -pedantic -Wall -g
OBJS= card.o 
# Deck.o Round.o Player.o

$(TARGET): $(OBJS)
	$(C  C) -o $(TARGET) $(OBJS)
	
card.o: card.c
	$(CC) $(CFLAGS) -c card.c
	
#sortmain.o: sortmain.c sort.h
#	$(CC) $(CFLAGS) -c sortmain.c
#	
#vector.o: vector.c ADTDefs.h
#	$(CC) $(CFLAGS) -c vector.c
	
clean:
	rm -f $(TARGET) $(OBJS)
