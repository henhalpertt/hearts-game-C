#ifndef __CARDS_H__
#define __CARDS_H__
#include "Errors.h"
typedef struct Game Game;
typedef   int(*GameStatus)(struct Game*);

struct Input
{
	size_t m_decks;
	size_t m_Noobs;
	size_t nBots;
}

struct Game
{
	Deck *m_deck;
	int m_nPlayers;
	GameStatus m_status;
}


SetGame(struct Input);

/*RunGame();*/

/*EndGame();*/

# endif /* __CARDS_H__ */




