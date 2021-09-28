#ifndef __PLAYER_H__
#define __PLAYER_H__
#define VALID_NUM_PLAYERS 4

typedef struct Team Team;
typedef struct Player Player; 
typedef struct Card Card;

enum PlayerSpecs
{
	PLAYER_MAGIC_NUM = 19931245,
	TEAM_MAGIC_NUM = 19931314,
	LOSER = 0,
	IN_THE_GAME = 1,
	OUT_OF_GAME = 0,
	REAL_PLAYER = 1,
	BOT_PLAYER  = 0
} PlayerSpecs;

void PrintCards(struct Team *_team);
void SortCards(struct Team *_team);
void SortCardsByRank(struct Team *_team);
void PlayerGiveCard(struct Team *_team, int _playerId, int *card, int(*PolicyGetCard)(int), int _cardIdx);
void GiveCardsToPlayer(struct Team *_team, int _playerId, int rank, int suit);
void FindPlayer(struct Team *_team, int _rank, int _suit, int *playerID);
void FindIdx(struct Team *_team, int _rank, int _suit, int *idx, int _player);

void FindBestCardIdx(struct Team *_team, int _playerID, int _leadSuit, int _leadRank, int *idx);

struct Team * CreatePlayers(int _nBots, int _nHumans, int _nCards, struct Card **_cards);

void DestroyTeam(struct Team *_team);

#endif /* __PLAYER_H__ */
