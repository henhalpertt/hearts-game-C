#ifndef __GAME_H__
#define __GAME_H__
#include "CardSpecs.h"

struct Game;

/*
	desc: Create a Game struct. each game consists of:
													- Team(struct);
													- # of bots(int);
													- # of humans(int);
													Game status(int); 
													pointer to scores(array of ints); 
													ID of player who won(int)
													hearts status - BROKEN/NOT_BROKEN;
	args: 
		-_nBots: # of bot players.
		-_nHumans:#of  human players. 
	output: pointer to Game struct.
	error handling:
		- returns NULL if malloc for team pointer failed.
		- returns NULL if malloc for players pointer failed.
		- retunrs NULL if _nHumans + _nBots smaller than or equals zero.
*/
struct Game * CreateGame(size_t _nBots, size_t _nHumans);

/*
	desc: Get Deck struct. each Deck holds n amount of cards that are already shuffled.
	args: 
		-_nCards: # of cards in the deck. 
	output: pointer to Deck struct.
	error handling:
*/
/*struct Deck * GetDeckForHearts(int _nCards);*/

/*
	desc: Calling SortCards function in a different module. Please refer to player.h;
	output:
	error handling:
*/
void SortHands(struct Game *_game);

/*
	desc: Calling SortCardsByRank function in a different module. Please refer to player.h;
	output:
	error handling:
*/
void SortHandsByRank(struct Game *_game);

/*
	desc: Calling PlayerGiveCard function in a different module. Please refer to player.h;
	output:
	error handling:
*/
void GetCardFromPlayer(struct Game *_game, size_t _playerId, int *card, size_t(*PolicyGetCard)(size_t), size_t _cardIdx);

/*
	desc: Calling CheckSuitInHand function in a different module. Please refer to player.h;
	output: returns SUIT_NOT_FOUND or SUIT_AVAILABLE
	error handling:
*/
int CheckSuitGame(struct Game *_game, size_t _playerID, Suit _leadSuit);

/*
	desc: Calling PlayerSeeCard function in a different module. Please refer to player.h;
	output:
	error handling:
*/
void SeeCardGame(struct Game *_game, size_t _playerId, int *card, size_t _idx);

/*
	desc: Calling GiveCardsToPlayer function in a different module. Please refer to player.h;
	output:
	error handling:
*/
void GiveCardsToGame(struct Game *_game, size_t _playerId, Rank rank, Suit suit);

/*
	desc: Calling FindPlayer function in a different module. Please refer to player.h;
	output:
	error handling:
*/
void FindPlayerGame(struct Game *_game, Rank _rank, Suit _suit, size_t *playerID);

/*
	desc: Calling FindIdx function in a different module. Please refer to player.h;
	output:
	error handling:
*/
void FindIdxGame(struct Game *_game, Rank _rank, Suit _suit, size_t *idx, size_t _player);

/*
	desc: Calling FindBestCardIdx function in a different module. Please refer to player.h;
	args: 
		-_game: pointer to struct Game.
	output:
	error handling:
*/
void FindBestCardGame(struct Game *_game, size_t _playerID, Suit _leadSuit, Rank _leadRank, size_t *idx, int _status);

/*
	desc: Update the score after each round.
	args: 
		-_game: pointer to struct Game.
		-_updatedScores: new scores to be addded to existing scores in game.
		-*GameStatus: pointer to function.
		  The returns value will determine the continuation of Game struct , or destruction of Game struct.  
	output: returns GAME_OVER or GAME_RUNNING.
	error handling:
		- returns ERR_NULL_SCORES if _updatedScores = NULL.
*/
int UpdateScores(struct Game *_game, int *_updatedScores, int(*GameStatus)(int *));

/*
	desc: free Deck struct member in Game and create a new Deck.
		  This will Create a new shuffled deck.
	args: 
		-_game: pointer to struct Game.
	output:
	error handling:
*/
void RefillDeck(struct Game *_game);

/*
	desc: Free game struct and all its members. 
	args: 
		-_game: pointer to struct Game.
	output:
	error handling:
*/
void DestroyGame(struct Game *_game);


/* Calling PrintCards function in a different module. Please refer to player.h  */ 
void PrintGameCards(struct Game *_game);

/* Calling PrintCardsHand function in a different module. Please refer to player.h; */
void PrintHand(struct Game *_game, size_t _playerId);

# endif /* __GAME_H__ */


