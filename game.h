#ifndef __GAME_H__
#define __GAME_H__
typedef struct Game Game;
/*typedef struct Team Team;*/

void PrintGameCards(struct Game *_game);
void PrintHand(struct Game *_game, int _playerId);

void SortHands(struct Game *_game);
void SortHandsByRank(struct Game *_game);
void GetCardFromPlayer(struct Game *_game, int _playerId, int *card, int(*PolicyGetCard)(int), int _cardIdx);
int CheckSuitGame(struct Game *_game, int _playerID, int _leadSuit);

void SeeCardGame(struct Game *_game, int _playerId, int *card, int _idx);
void GiveCardsToGame(struct Game *_game, int _playerId, int rank, int suit);

void FindPlayerGame(struct Game *_game, int _rank, int _suit, int *playerID);

void FindIdxGame(struct Game *_game, int _rank, int _suit, int *idx, int _player);

void FindBestCardGame(struct Game *_game, int _playerID, int _leadSuit, int _leadRank, int *idx, int _status);

int UpdateScores(struct Game *_game, int *_updatedScores, int(*GameStatus)(int *));

void RefillDeck(struct Game *_game);

struct Game * CreateGame(int _nBots, int _nHumans);
struct Deck * GetDeckForHearts(int _nCards);
int GetAmntOfCardsHearts();

void DestroyGame(struct Game *_game);

/* RunGame(); */

/* EndGame(); */

# endif /* __GAME_H__ */


