# makefile sample

TARGET= t.out
CC= gcc
CFLAGS= -ansi -pedantic -Wall -g
OBJS= main.o 
# Deck.o Round.o Player.o

$(TARGET): $(OBJS)
	$(C  C) -o $(TARGET) $(OBJS)
	
main.o: main.c
	$(CC) $(CFLAGS) -c main.c
	
#sortmain.o: sortmain.c sort.h
#	$(CC) $(CFLAGS) -c sortmain.c
#	
#vector.o: vector.c ADTDefs.h
#	$(CC) $(CFLAGS) -c vector.c
	
clean:
	rm -f $(TARGET) $(OBJS)
