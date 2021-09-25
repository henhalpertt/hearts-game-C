/* possible NULL pointers */
#include <stdio.h>
/* malloc for a new cards */
#include <stdlib.h>
/* status signals */

struct Game
{
	int m_gameStatus; 
	int *m_scores; 
	int m_whosTheWinner;
};

struct Game * CreateGame(int _nPlayers)
{
	struct Game *newGame;
	int *scores;
	int player;
	/* new game */
	newGame = (struct Game*)malloc(sizeof(struct Game));
	if(newGame == NULL)
	{
		return;
	}
	scores = (int*)malloc(sizeof(int) * _nPlayers);
	if(scores == NULL)
	{
		return;
	}
	/* init game */
	newGame->m_scores = scores;
	for(player=0; player < _nPlayers; player++ )
	{
		newGame->m_scores[player] = 0; /*initial score of each player */
	}
	return newGame;
	/* POLICY Functions */
	 
	
}




