#ifndef __GAME_H__
#define __GAME_H__
typedef struct Game Game;
typedef struct Team Team;

void PrintGameCards(struct Game *_game);
void SortHands(struct Game *_game);
void SortHandsByRank(struct Game *_game);
void GetCardFromPlayer(struct Game *_game, int _playerId, int *card, int(*PolicyGetCard)(int), int _cardIdx);
void GiveCardsToGame(struct Game *_game, int _playerId, int rank, int suit);

void FindPlayerGame(struct Game *_game, int _rank, int _suit, int *playerID);

void FindIdxGame(struct Game *_game, int _rank, int _suit, int *idx, int _player);

struct Game * CreateGame(int _nBots, int _nHumans);
struct Deck * GetDeckForHearts(int _nCards);
int GetAmntOfCardsHearts();
/* RunGame(); */

/* EndGame(); */

# endif /* __GAME_H__ */


