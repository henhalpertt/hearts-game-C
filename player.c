/* possible NULL pointers */
#include <stdio.h>
/* malloc for a new cards */
#include <stdlib.h>
#include "card.h"
#include "UI.h"
#define SUIT_AVAILABLE 1
#define SUIT_NOT_FOUND 0
#define LIMIT 13
#define BROKEN 0
#define UNBROKEN 1

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

struct Player
{
	struct Card **m_cards; 
	int m_nCardsInHand;
	int m_id; /* player id [ 1 upto nth players, according to some policy(in our case, 4 players)] */
	int m_realOrNot; /* bot or human */
	int m_inOrOutOfTheGame; /* other games night continue, 
								exluding some players */
	int m_winner; /* all of them are losers at first, flg will change 
					once game is over only to one of the players */
	int m_magic;
};

struct Team
{
	struct Player **m_players;
	int m_totalPlayers;
	int m_magic;
};

/* ---- HELPER FUNCTIONS ---- */ 
static struct Player * CreatePlayer(int _id, int _from, int _nCards, struct Card **_cards)
{
	struct Player *newplayer;
	int card;
	Rank rank;
	Suit suit;
	
	newplayer = (struct Player*)malloc(sizeof(struct Player));
	if(newplayer == NULL)
	{
		return NULL;
	}
	newplayer->m_id = _id;
	newplayer->m_realOrNot = BOT_PLAYER;
	newplayer->m_inOrOutOfTheGame = IN_THE_GAME;
	newplayer->m_winner = LOSER;
	newplayer->m_magic = PLAYER_MAGIC_NUM;
	newplayer->m_nCardsInHand = _nCards;
	newplayer->m_cards = _cards;

	for(card= 0; card < _nCards; card++)
	{
		rank = GetRank(newplayer->m_cards[card]);
		suit = GetSuit(newplayer->m_cards[card]);
		newplayer->m_cards[card] = CreateCard(suit, rank);
	}
	return newplayer;
}

static void MakePlayers(struct Team *_newTeam, int _nPlayers, int _nCards, struct Card **_cards)
{
	int player, from;
	if(_nPlayers == 0)
	{
		return;
	}
	for(player=0, from=0; player<_nPlayers; player++, from+=13)
	{
		_newTeam->m_players[player] = CreatePlayer(player, from, _nCards, _cards+from);
	}
}

static int FindMinInSubset(struct Player *_player, int _idx)
{
	int tmp, suitItem, rankItem, item;
	int minIdx=_idx;
	
	
	tmp = 100 * GetSuit(_player->m_cards[_idx]) + GetRank(_player->m_cards[_idx]);
	for(;_idx<_player->m_nCardsInHand; _idx++)
	{
		suitItem = 100 * GetSuit(_player->m_cards[_idx]);
		rankItem = GetRank(_player->m_cards[_idx]);
		item = suitItem + rankItem;

		if(tmp > item)
		{
			minIdx = _idx;
			tmp = item;
		}
	}
	return minIdx;
}

static int FindMinInSubsetByRank(struct Player *_player, int _idx)
{
	int tmp, rankItem;
	int minIdx=_idx;
	
	tmp = GetRank(_player->m_cards[_idx]);
	for(;_idx<_player->m_nCardsInHand; _idx++)
	{
		rankItem = GetRank(_player->m_cards[_idx]);
		if(tmp > rankItem)
		{
			minIdx = _idx;
			tmp = rankItem;
		}
	}
	return minIdx;
}

static void SortEachHand(struct Player *_player)
{
	int card, minIdx;
	int item;
	
	/* for every card in hand  */
	for(card=0; card<_player->m_nCardsInHand-1; card++)
	{
		/* selection sort */
		minIdx = FindMinInSubset(_player, card);
		SwapCards(_player->m_cards[minIdx], _player->m_cards[card]);
	}
}

/* ---- API Functions ---- */
void SortEachHandByRank(struct Player *_player)
{
	int card, minIdx;
	int item;
	
	/* for every card in hand  */
	for(card=0; card<_player->m_nCardsInHand-1; card++)
	{
		/* selection sort */
		minIdx = FindMinInSubsetByRank(_player, card);
		SwapCards(_player->m_cards[minIdx], _player->m_cards[card]);
	}
}



void SortCards(struct Team *_team)
{
	int player;
	for(player=0; player<_team->m_totalPlayers; player++)
	{
		SortEachHand(_team->m_players[player]);
	}
}

void SortCardsByRank(struct Team *_team)
{
	int player;	
	for(player=0; player<_team->m_totalPlayers; player++)
	{
		SortEachHandByRank(_team->m_players[player]);
	}
}

void PlayerGiveCard(struct Team *_team, int _playerId, int *card, int(*PolicyGetCard)(int _idx), int _idx)
{
	int _cardIdx;
	int i;
	_cardIdx = PolicyGetCard(_idx);
	if(_cardIdx > LIMIT || _cardIdx < 0)
	{
		return;
	}
	card[0] = GetSuit(_team->m_players[_playerId]->m_cards[_cardIdx]);
	card[1] = GetRank(_team->m_players[_playerId]->m_cards[_cardIdx]);
	for(i = _cardIdx; i<(_team->m_players[_playerId]->m_nCardsInHand-1); i++)
	{
		_team->m_players[_playerId]->m_cards[i] = _team->m_players[_playerId]->m_cards[i+1];
	}
	_team->m_players[_playerId]->m_nCardsInHand--;
}

void PlayerSeeCard(struct Team *_team, int _playerId, int *card, int _idx)
{
	int _cardIdx;
	_cardIdx = PolicyGetCard(_idx);
	if(_cardIdx > LIMIT || _cardIdx < 0)
	{
		return;
	}
	card[0] = GetSuit(_team->m_players[_playerId]->m_cards[_cardIdx]);
	card[1] = GetRank(_team->m_players[_playerId]->m_cards[_cardIdx]);
}

void GiveCardsToPlayer(struct Team *_team, int _playerId, int rank, int suit)
{
	int nCardsInHand;
	nCardsInHand = _team->m_players[_playerId]->m_nCardsInHand;
	_team->m_players[_playerId]->m_cards[nCardsInHand] = CreateCard(suit, rank);
	_team->m_players[_playerId]->m_nCardsInHand++;
}

void FindPlayer(struct Team *_team, int _rank, int _suit, int *playerID)
{
	int totalNPlayers, card;
	int suit, rank, player;
	totalNPlayers = _team->m_totalPlayers;
	
	for(player=0; player<totalNPlayers; player++)
	{
		for(card=0; card<_team->m_players[player]->m_nCardsInHand; card++)
		{
			rank = GetRank(_team->m_players[player]->m_cards[card]);
			suit = GetSuit(_team->m_players[player]->m_cards[card]);
			if(suit == _suit && rank == _rank)
			{
				*playerID = player;
				return;
			}
		}
	}
}

void FindIdx(struct Team *_team, int _rank, int _suit, int *idx, int _player)
{
	int totalNPlayers, card;
	int suit, rank, player;
	totalNPlayers = _team->m_totalPlayers;
	for(card=0; card<_team->m_players[_player]->m_nCardsInHand; card++)
	{
		rank = GetRank(_team->m_players[_player]->m_cards[card]);
		suit = GetSuit(_team->m_players[_player]->m_cards[card]);
		if(suit == _suit && rank == _rank)
		{
			*idx = card;
			return;
		}
	}
	*idx = 0;
}

int  CountSuit(struct Player *_player ,int _suit)
{
	int card, suit, cnt;
	cnt = 0;
	for(card=0; card<_player->m_nCardsInHand; card++)
	{
		suit = GetSuit(_player->m_cards[card]);
		if(_suit == suit)
		{
			cnt++;
		}
	}
	return cnt;
}

int AvailabilityOfSuit(struct Player *_player, int _leadSuit)
{
	int card, suit;
	for(card=0; card<_player->m_nCardsInHand; card++)
	{
		suit = GetSuit(_player->m_cards[card]);
		if(suit == _leadSuit)
		{
			return SUIT_AVAILABLE;
		}
	}
	return SUIT_NOT_FOUND;
}

int CheckSuitInHand(struct Team *_team, int _playerID, int _leadSuit, int *result)
{
	if(AvailabilityOfSuit(_team->m_players[_playerID], _leadSuit) == SUIT_NOT_FOUND)
	{
		return SUIT_NOT_FOUND;
	}
	return SUIT_AVAILABLE;
}

void FindBestCardIdx(struct Team *_team, int _playerID, int _leadSuit, int _leadRank, int *idx, int status)
{
	int size, tmp=0, card;
	int suit, rank;
	int flg=0;

	if(AvailabilityOfSuit(_team->m_players[_playerID],_leadSuit) == SUIT_NOT_FOUND)
	{
		SortEachHandByRank(_team->m_players[_playerID]);
		if(status == BROKEN)
		{
			/* put low rank cards - cautious policy */
			*idx = 0;
		}
		else
		{
			/* put high rank cards - risky policy */
			*idx = _team->m_players[_playerID]->m_nCardsInHand-1;
		}
		return;
	}
	/* suit found. look for rank cards closest to LeadingRank */ 
	for(card=0; card<_team->m_players[_playerID]->m_nCardsInHand; card++)
	{
		suit = GetSuit(_team->m_players[_playerID]->m_cards[card]);
		rank = GetRank(_team->m_players[_playerID]->m_cards[card]);
		
		if(suit == _leadSuit && rank < _leadRank)
		{
			*idx = card;
			flg=1;
		}
	}
	if(flg == 1)
	{
		return;
	}
	/* rank cards are higher than leading cards, pick one that is closest to leadingRank */ 
	for(card=0; card<_team->m_players[_playerID]->m_nCardsInHand; card++)
	{
		suit = GetSuit(_team->m_players[_playerID]->m_cards[card]);
		rank = GetRank(_team->m_players[_playerID]->m_cards[card]);
		
		if(suit == _leadSuit && rank > _leadRank)
		{
			*idx = card;
			return;
		}
	}
}

struct Team * CreatePlayers(int _nBots, int _nHumans, int _nCards, struct Card **_cards)
{
	struct Player **newPlayers;
	struct Team *newTeam;
	int sizeOfPlayers;
	newTeam = (struct Team*)calloc(1, sizeof(struct Team));
	if(newTeam == NULL)
	{
		return NULL;
	}
	newPlayers = (struct Player**)calloc(_nBots + _nHumans, sizeof(struct Player));
	if(newPlayers == NULL)
	{
		return NULL;
	}
	newTeam->m_players = newPlayers;
	newTeam->m_totalPlayers = _nHumans + _nBots;
	newTeam->m_magic = TEAM_MAGIC_NUM;
	MakePlayers(newTeam, newTeam->m_totalPlayers, _nCards, _cards);
	return newTeam;
}

void DestroyTeam(struct Team *_team)
{
	int p;
	if(_team == NULL || _team->m_magic != TEAM_MAGIC_NUM)
	{
		return;
	}
	_team->m_magic = 0;
	for(p=0; p<_team->m_totalPlayers; p++)
	{
		free(_team->m_players[p]);
	}
	free(_team);
}

/* ---  PRINTS --- */

void PrintCards(struct Team *_team)
{
	int card, player;
	for(player=0; player<4; player++)
	{
		PrintIdUI(_team->m_players[player]->m_id);
		for(card=0; card<(_team->m_players[player]->m_nCardsInHand); card++)
		{
			PrintCardUI(_team->m_players[player]->m_cards[card]);
			PrintStrUI(NEWLINE);
		}
		PrintStrUI(NEWLINE);
	}
}

void PrintCardsHand(struct Team *_team, int _playerId)
{
	int card;
	PrintIdUI(_playerId);
	for(card=0; card<(_team->m_players[_playerId]->m_nCardsInHand); card++)
	{
		PrintCardIdxUI(card);
		PrintCardUI(_team->m_players[_playerId]->m_cards[card]);
	}
	PrintStrUI(NEWLINE);
}

/* END */


