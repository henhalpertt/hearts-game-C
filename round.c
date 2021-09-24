/* possible NULL pointers */
#include <stdio.h>
/* malloc for a new cards */
#include <stdlib.h>
#define TRICK_MAGIC_NUM 1435654
#define ROUND_MAGIC_NUM 2334543
#define ROUNDS_MAGIC_NUM 2983643
#define FIRST_TRICK 1

#include "deck.h"
#include "player.h"
#include "game.h"

struct Input
{
	size_t m_nCards;
	size_t m_nBots;
	size_t m_nHumans;
};

struct InputForTrick
{
	struct Team *m_players;
	struct Deck *m_cards;
	struct Game *m_game;
};

/* structs */
struct Hands
{
	int m_playerId;
	size_t m_nCardsInHand;
	struct Card **m_cards;
};

struct Trick
{
	struct Team *m_team;
	struct Deck *m_deck;
	struct Game *m_game;
	struct Hand **m_hands;
	
	int *m_penalties; /* score of each player at the end of trick
							size varies according to # of players */
	int m_trickNumber; /* 1-13 tricks */
	int m_playerWClubsTwo; /* the player that is holding 2 of clubs */
	int m_playerWSpadesQueen; /* the player that is holding queen of spades */
	int m_heartsStatus; /* was hearts broken or is it still unbroken */
	int m_magic;
};

struct Round
{
	struct Trick **m_tricks; /* each round has 13 tricks */
	int m_roundNumber; /* round 1, ... n rounds */
	int m_ovreallPenalties; /* scores for each player at the end of round */
	int m_currentWinner; /*the player w/ least m_penalties pts */
	int m_heartsStatus;
	int m_magic;
};

struct Rounds
{
	struct Round **m_rounds;
	int m_magic;
};


static CreateHand(struct Trick *_trick, int _playerID, int _nCardsInHand)
{
	struct Hand *newHand;
		
	newHand = (struct Hand*)malloc(struct Hand);
	if(newHand == NULL)
	{
		return NULL;
	}
	newHand->m_playerId = _playerID;
	newHand->m_nCardsInHand = _nCardsInHand;
	
	
	
}
static struct Trick * CreateTrick(int _trickNum, struct InputForTrick _in)
{
	struct Trick *newTrick;
	struct Deck *myDeck;
	struct Team *myPlayers;
	
	newTrick = (struct Trick*)malloc(sizeof(struct Trick));
	if(newTrick == NULL)
	{
		return NULL;
	}

	newTrick->m_team = _in.m_players;
	newTrick->m_deck = _in.m_cards;
	newTrick->m_game = _in.m_game;
	
	newTrick->m_trickNumber = _trickNum;
	newTrick->m_playerWClubsTwo = 0;
	newTrick->m_playerWSpadesQueen = 0;
	newTrick->m_heartsStatus = 0;
	newTrick->m_magic = TRICK_MAGIC_NUM;
	return newTrick;
}

static void SplitCardsToPlayers(struct Trick *_trick)
{
	int player,card, nCards, nPlayers;
	if(_trick == 0)
	{
		return;
	}
	nCards = _trick->m_deck->m_nCards;
	nPlayers = _trick->m_team->m_totalPlayers;
	
	for(card=0, player=0; card<nCards; card++)
	{
			if(card % (nCards/nPlayers) == 0 && card != 0)
			{
				printf("\n");
				player++;
			}
			printf("Player:%d card:%d, ", _trick->m_team->m_players[player]->m_id,
										  _trick->m_deck->m_cards[card]->m_rank);
						  
	}	
}

/*static void SortBySuitAndRank(what to )*/
/*{*/
/*	*/
/*}*/


struct Trick * SetUpGame(struct Input *_in)
{
	int nCards, nBots, nHumans, totalPlayers;
	size_t suit, rank, cardCnt;
	size_t player; /*idx */
	struct Deck *newDeck;
	struct Team *newTeam;
	struct Game *newGame;
	struct InputForTrick input;
	struct Trick *firstTrick;
	
	if (_in == NULL)
	{
		return;
	}
	nCards = _in->m_nCards;
	nBots = _in->m_nBots;
	nHumans = _in->m_nHumans;
	
	/* Create Deck */
	newDeck = CreateDeck(nCards);
	cardCnt=0;
	for(suit=HEARTS; suit<=CLUBS; suit++)
	{
		for(rank=TWO; rank<=ACE; rank++)
		{
			printf("s:%d,r:%d ",
						 newDeck->m_cards[cardCnt]->m_suit, 
								newDeck->m_cards[cardCnt]->m_rank);
			cardCnt++;
		}
		printf("\n");
	}
	/* Create players */
	newTeam = MakePlayers(nBots, nHumans);
	totalPlayers = newTeam->m_totalPlayers;
	for(player=0; player<totalPlayers; player++)
	{
		printf("\nplayer id: %d, real? %d, inOrOut? %d, winner? %d \n",
					newTeam->m_players[player]->m_id,
					newTeam->m_players[player]->m_realOrNot,
					newTeam->m_players[player]->m_inOrOutOfTheGame,
					newTeam->m_players[player]->m_winner);
	}
	/*Create a Game*/
	newGame = CreateGame(totalPlayers);
	for(player=0; player<totalPlayers; player++)
	{
		printf("\nplayer initial score: %d\n", newGame->m_scores[player]);
	}
	input.m_players = newTeam;
	input.m_cards = newDeck;
	input.m_game = newGame;
	
	firstTrick = CreateTrick(FIRST_TRICK, input);
	SplitCardsToPlayers(firstTrick);
}


/* END */
































