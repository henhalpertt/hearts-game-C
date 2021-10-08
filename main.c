#include <stdio.h>
#include "SpecialChars.h"
#define ONE_PLAYER 1
#define NO_REAL_PLAYERS 0
int main(void)
{
	size_t numPlayers;
	PrintStrUI(ENTER_NUM_PLAYERS);
	UserInput(&numPlayers);
	SetUpGame(numPlayers); /*how many (real) players were chosen to play this game? */
	return 0;
}


/* TO DO */
/* round and game - some variable require a size_t, not an int ! */ 

/* atoi for user input */

/* clean code and break down RunTrick in round.c into helpers */ 

/* ptr to function - use void * to pass params. implement for round.c <--> player.c */  


