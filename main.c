#include <stdio.h>
#include "game.h"

int main(void)
{
	struct Input in[PARAMS] = {52, 1, 0}; /* 52 cards, 1 bot, 0 humans */
	CreateGame(in);
	
	RunGame();
	
	DestroyGame();
}
