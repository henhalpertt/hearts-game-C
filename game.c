/* possible NULL pointers */
#include <stdio.h>
/* malloc for a new cards */
#include <stdlib.h>
/* catching errors */
#include "Errors.h"
/* status signals */
#include "Signal.h"
struct Input
{
	size_t m_nCards;
	size_t m_nBots;
	size_t m_nHumans;
}

struct Game
{
	Deck *m_deck;
/*	Player *m_players;*/
	GameStatus m_status;
	int m_nBots;
	int m_nHumans
}

void SetGame(struct Input _in)
{
	struct Deck *deck;
	struct Game *newGame;
/*	struct Round *round;*/
/*	struct Players *players;*/
	
/*	struct Players *_player;*/
	
	deck = GetDeck(_in->m_nCards);
	newGame = (struct Game*)malloc(sizeof(struct Game));
	if(newGame == NULL)
	{
		return NULL;
	}
	/* init game */
	newGame->m_deck = deck;
	newGame->m_status = CheckStatus(NEW_GAME);
	newGame->m_bots = _in->m_nBots;
	newGame->m_nHumans = _in->m_nHumans;
	
	/* POLICY - Round Functions */
	PrerequisitesForRound(newGame); /* splitting cards to players,
												 sorting by suit+rank,
												 				passing 3 cards among players */
	
	
										 					
}









