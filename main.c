#include <stdio.h>
#include "round.h"
#define PARAMS 3
struct Input
{
	size_t m_nCards;
	size_t m_nBots;
	size_t m_nHumans;
};

int main(void)
{
	struct Input in[PARAMS] = {52, 4, 0}; /* 52 cards, 4 bots, 0 humans */
	 SetUpGame(in); /* create game, cards, players, define rules */ 
	 return 0;
}
/*	RunGame();*/
/*	*/
/*	DestroyGame();*/
