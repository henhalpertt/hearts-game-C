#ifndef __GAME_H__
#define __GAME_H__
typedef struct Game Game;

struct Game
{
	int m_gameStatus; /* 0 - still running, 1 - game over */
	int *m_scores; /* follow-up on scores */
	int m_whosTheWinner; /* '1' if someone won. otherwise '0' to all players */
};

struct Game * CreateGame(int _nPlayers);

/* RunGame(); */

/* EndGame(); */

# endif /* __GAME_H__ */
