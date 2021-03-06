/* possible NULL pointers */
#include <stdio.h>
/* malloc for a new cards */
#include <stdlib.h>
#include "SpecialChars.h"
#include "CardSpecs.h"
#include "round.h"
#include "player.h"
#include "deck.h"
#include "card.h"

#define GAME_MAGIC_NUM 1266784
#define NO_WINNER_YET -1
#define ERR_NULL_SCORES -2
#define STILL_RUNNING 0
#define RUNNING_GAME 1
#define GAME_OVER 100

struct Game
{
	struct Team *m_team;
	size_t m_bots;
	size_t m_humans;
	int m_gameStatus; 
	int *m_scores; 
	int m_status;
	int m_magic;
};

void RefillDeck(struct Game *_game)
{
	struct Deck *newDeck;
	struct Team *newTeam;
	int nCards, nCardsInHand;
	nCards = GetAmntOfCardsHearts();
	newDeck = GetDeckForHearts(nCards);
	nCardsInHand = nCards / (_game->m_bots + _game->m_humans);
	DestroyTeam(_game->m_team); 
	newTeam = CreatePlayers(_game->m_bots, _game->m_humans, nCardsInHand, GetCards(newDeck));
	_game->m_team = newTeam;
	
}

struct Game * CreateGame(size_t _nBots, size_t _nHumans) /* called in main.c */
{
	struct Game *newGame;
	struct Team *newTeam;
	struct Deck *newDeck;
	int *scores;
	size_t player;
	size_t nCards;
	size_t nCardsInHand;
	/* new game */
	newGame = (struct Game*)malloc(sizeof(struct Game));
	if(newGame == NULL)
	{
		return NULL;
	}
	scores = (int*)malloc(sizeof(int) * (_nBots + _nHumans));
	if(scores == NULL)
	{
		return NULL;
	}
	/* create a deck according to policy */
	nCards = GetAmntOfCardsHearts();
	newDeck = GetDeckForHearts(nCards);
	
	/*create players */
	nCardsInHand = nCards / (_nBots + _nHumans);
	newTeam = CreatePlayers(_nBots, _nHumans, nCardsInHand, GetCards(newDeck)); 
	newGame->m_team = newTeam;
	newGame->m_status = STILL_RUNNING;
	newGame->m_scores = scores;
	newGame->m_bots = _nBots;
	newGame->m_humans = _nHumans;
	newGame->m_magic = GAME_MAGIC_NUM;
	for(player=0; player < (_nBots + _nHumans); player++ )
	{
		newGame->m_scores[player] = 0; /*initial score of each player */
	}
	return newGame;
}

void SortHands(struct Game *_game)
{
	SortCards(_game->m_team);
}

void SortHandsByRank(struct Game *_game)
{
	SortCardsByRank(_game->m_team);
}

void PrintGameCards(struct Game *_game)
{
	PrintCards(_game->m_team);
}

void PrintHand(struct Game *_game, size_t _playerId)
{
	PrintCardsHand(_game->m_team, _playerId);
}

void GetCardFromPlayer(struct Game *_game, size_t _playerId, int *card, size_t(*PolicyGetCard)(size_t), size_t _cardIdx)
{
	PlayerGiveCard(_game->m_team, _playerId, card, PolicyGetCard, _cardIdx);
}

void SeeCardGame(struct Game *_game, size_t _playerId, int *card, size_t _idx)
{
	PlayerSeeCard(_game->m_team, _playerId, card, _idx);
}

void GiveCardsToGame(struct Game *_game, size_t _playerId, Rank rank, Suit suit)
{
	GiveCardsToPlayer(_game->m_team, _playerId, rank, suit);
}

void FindPlayerGame(struct Game *_game, Rank _rank, Suit _suit, size_t *playerID)
{
	FindPlayer(_game->m_team, _rank, _suit, playerID);
}

void FindIdxGame(struct Game *_game, Rank _rank, Suit _suit, size_t *idx, size_t _player)
{
	FindIdx(_game->m_team, _rank, _suit, idx, _player);
}

void FindBestCardGame(struct Game *_game, size_t _playerID, Suit _leadSuit, Rank _leadRank, size_t *idx, int _status)
{
	FindBestCardIdx(_game->m_team, _playerID, _leadSuit, _leadRank, idx, _status);
}

int CheckSuitGame(struct Game *_game, size_t _playerID, Suit _leadSuit)
{
	return CheckSuitInHand(_game->m_team, _playerID, _leadSuit);
}

void PrintBoard(struct Game *_game)
{
	size_t nPlayers;
	int score;
	nPlayers = _game->m_bots + _game->m_humans;
	for(score=0; score<nPlayers; score++)
	{
		PrintPlayerAndScore(score, _game->m_scores[score]);
	}
}

void DestroyGame(struct Game *_game)
{
	if(_game == NULL || _game->m_magic != GAME_MAGIC_NUM)
	{
		return;
	}
	_game->m_magic = 0;
	DestroyTeam(_game->m_team);
	free(_game);
}

size_t GetWinner(struct Game *_game)
{
	int tmp, score;
	size_t minIdx=0;
	size_t nPlayers;
	nPlayers = _game->m_bots + _game->m_humans;
	
	tmp = _game->m_scores[0];
	for(score=0; score<nPlayers; score++)
	{
		if(_game->m_scores[score] < tmp)
		{
			tmp = _game->m_scores[score];
			minIdx = score;
		}
	}
	return minIdx;
}

int UpdateScores(struct Game *_game, int *_updatedScores, int(*GameStatus)(int *))
{
	size_t minIdx;
	size_t nPlayers;
	int score;
	if(_updatedScores == NULL)
	{
		return ERR_NULL_SCORES;
	}
	nPlayers = _game->m_bots + _game->m_humans;
	PrintStrUI(SCORE_BOARD);
	for(score=0; score<nPlayers; score++)
	{
		_game->m_scores[score] += _updatedScores[score];
	}
	if(GameStatus(_game->m_scores) == GAME_OVER)
	{
		PrintStrUI(GAME_RESULTS);
		PrintBoard(_game);
		minIdx = GetWinner(_game);
		PrintValueUI(WINNER_IS, minIdx);
		DestroyGame(_game);
		return GAME_OVER;
	}
	printf("\nAccumulated Scores:\n");
	PrintBoard(_game);
	return RUNNING_GAME;
}




