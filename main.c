#include <stdio.h>
#include "SpecialChars.h"
#define ONE_PLAYER 1
#define NO_REAL_PLAYERS 0
int main(void)
{
	int numPlayers;
	PrintStrUI(ENTER_NUM_PLAYERS);
	UserInput(&numPlayers);
	SetUpGame(numPlayers); /*how many (real) players were chosen to play this game? */
	return 0;
}
