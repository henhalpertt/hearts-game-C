/* possible NULL pointers */
#include <stdio.h>
/* malloc for a new cards */
#include <stdlib.h>
#include "Errors.h"
#include "Signals.h"

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


struct Player * CreatePlayers(int _id, int _realOrNot)
{
	struct Player *newplayer;

	newplayer = (struct Player*)malloc(sizeof(struct Player));
	if(newplayer == NULL)
	{
		return NULL;
	}
	newplayer->m_id = _id;
	newplayer->m_realOrNot = _realOrNot;
	newplayer->m_inOrOutOfTheGame = IN_THE_GAME;
	newplayer->m_winner = LOSER;
	newplayer->m_magic = PLAYER_MAGIC_NUM;
	return newplayer;
}

void MakeHumanPlayers(struct Team *_newTeam, int _nHumans)
{
	int person;
	if(_nHumans == 0)
	{
		return;
	}
	for(person=0; person<_nHumans; person++)
	{
		_newTeam->m_players[person] = CreatePlayers(person+1,REAL_PLAYER);
		_newTeam->m_totalPlayers++;
	}
}

void MakeBotPlayers(struct Team *_newTeam, int _nBots)
{
	int bot, from;
	if(_nBots == 0)
	{
		return;
	}
	from = _newTeam->m_totalPlayers;
	for(bot=from; bot<_nBots+from; bot++)
	{
		_newTeam->m_players[bot] = CreatePlayers(bot+1,BOT_PLAYER);
		_newTeam->m_totalPlayers++;
	}
}

struct Team * MakePlayers(int _nBots, int _nHumans)
{
	struct Player **newPlayers;
	struct Team *newTeam;
	int sizeOfPlayers;
	if(_nHumans == 0 && _nBots == 0)
	{
		return NULL;
	}
	
	newTeam = (struct Team*)malloc(sizeof(struct Team));
	if(newTeam == NULL)
	{
		return NULL;
	}
	
	sizeOfPlayers = sizeof(struct Player) * (_nBots + _nHumans);
	newPlayers = (struct Player**)malloc(sizeOfPlayers);
	if(newPlayers == NULL)
	{
		return NULL;
	}
	
	newTeam->m_players = newPlayers;
	newTeam->m_totalPlayers = NEW_GAME;
	newTeam->m_magic = TEAM_MAGIC_NUM;
	
	/* make humans first */
	MakeHumanPlayers(newTeam, _nHumans);
	
	/* now make bot players */
	MakeBotPlayers(newTeam, _nBots);
	
	return newTeam;
}

int main(void)
{
	struct Team *newTeam;
	int player;
	
	newTeam = MakePlayers(1, 3); /* 3 bots 1 human */
	for(player=0; player<4; player++)
	{
		printf("\nplayer id: %d, real? %d, inOrOut: %d, winner? %d \n",
					newTeam->m_players[player]->m_id,
					newTeam->m_players[player]->m_realOrNot,
					newTeam->m_players[player]->m_inOrOutOfTheGame,
					newTeam->m_players[player]->m_winner);
	}
	return 0;
	
}

/* END */


