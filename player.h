#ifndef __PLAYER_H__
#define __PLAYER_H__
#define VALID_NUM_PLAYERS 4
enum PlayerSpecs
{
	PLAYER_MAGIC_NUM = 19931245,
	TEAM_MAGIC_NUM = 19931314,
	LOSER = 0,
	IN_THE_GAME = 1,
	OUT_OF_GAME = 0,
	REAL_PLAYER = 1,
	BOT_PLAYER  = 0,
} PlayerSpecs;
/* intrinsic properties of player only ! generic struct */
struct Player
{
	int m_id; /* player id [minimum 1 & upto nth players] */
	int m_realOrNot; /* bot or human */
	int m_inOrOutOfTheGame; /* other games night continue, 
								exluding some players */
	int m_winner; /* all of them are losers at first, flg will change 
					once game is over only to one of the players */
	int m_magic;
	
};

/* players as a whole --> Team */
struct Team
{
	struct Player **m_players;
	int m_totalPlayers;
	int m_magic;
};

typedef struct Team Team;
typedef struct Player Player;

struct Team * MakePlayers(int _nBots, int _nHumans);

#endif /* __PLAYER_H__ */
