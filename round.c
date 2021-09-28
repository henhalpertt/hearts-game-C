/* possible NULL pointers */
#include <stdio.h>
/* malloc for a new cards */
#include <stdlib.h>
#define TRICK_MAGIC_NUM 1435654
#define ROUND_MAGIC_NUM 2334543
#define ROUNDS_MAGIC_NUM 2983643
#define ZEROTH_TRICK 0
#define CARDS_EACH_HAND 13
#define FULL_DECK 52
#define BANK_SIZE 12
#define BROKEN 0
#define UNBROKEN 1
#define FULL_RND 13
#define TABLE_SIZE 4

#include "game.h"
#include "round.h"
#include "deck.h"
#include "card.h"
#include "player.h"

struct Trick
{
	struct Game *m_game; /* communicating game status and scores */
	int *m_penalties; /* score of each player at the end of trick
							size varies according to # of players */
	int m_trickNumber; /* 1-13 tricks */					
	int m_playerWClubsTwo; /* the player that is holding 2 of clubs ie starting the game */
	int m_playerWSpadesQueen; /* the player that is holding queen of spades */
	int m_leadingSuit; /* the leading suit for the trick */
	int m_startingPlayer; /* if this isnt first trick, starting player = player who "won" prev trick */
	int m_heartsStatus; /* was hearts broken or is it still unbroken */
	int m_magic;
};

struct Round
{
	struct Trick **m_tricks; /* each round has 13 tricks */
	int m_roundNumber; /* round 1, ... n rounds */
	int m_ovreallPenalties; /* scores for each player at the end of round (13 tricks) */
	int m_currentWinner; /*the player w/ least m_penalties pts */
	int m_heartsStatus;
	int m_magic;
};

struct Rounds 
{
	struct Round **m_rounds;
	int m_magic;
};


static struct Trick * CreateTrick(int _trickNum)
{
	struct Trick *newTrick;
	struct Deck *myDeck;
	struct Team *myPlayers;
	
	newTrick = (struct Trick*)malloc(sizeof(struct Trick));
	if(newTrick == NULL)
	{
		return NULL;
	}
	
	newTrick->m_game = CreateGame(4, 0);
	
	newTrick->m_trickNumber = _trickNum;
	newTrick->m_playerWClubsTwo = -1;
	newTrick->m_playerWSpadesQueen = -1;
	newTrick->m_heartsStatus = UNBROKEN;
	newTrick->m_magic = TRICK_MAGIC_NUM;
	newTrick->m_leadingSuit = -1;
	newTrick->m_startingPlayer = -1;
/*	newTrick->m_bank = NULL;*/
	newTrick->m_penalties = NULL;
	return newTrick;
}

int func1(int n1, int n2)
{
	if (n1 < n2)
	{
		return 1;
	}
}

void func2(int *a, int *b, int *c, int *d)
{
	*a = 2;
	*b = 3;
	*c = 4;
	*d = 5;	
}

struct Deck * GetDeckForHearts(int _nCards)
{
	struct Deck *newDeck;
	newDeck = CreateDeck(_nCards);
	return newDeck;
}

int GetAmntOfCardsHearts()
{
	return FULL_DECK;
}

/* --------------- PASS THREE CARDS ---------------- */

int PolicyGetCard(int _cardIdx)
{
	/* return the  card idx  and to get rid of the cards */
	return _cardIdx;
}

int PolicyPutOnTable(int _cardIdx) /* add trick number ad param*/
{
	return _cardIdx;
}

/* ------------------               ----------------------*/

struct Card * GetCard(struct Game *_game, int _playerId, int *card, int(*Policy)(int), int _cardIdx)
{
	GetCardFromPlayer(_game, _playerId, card, Policy, _cardIdx);
}

void GiveCards(struct Game *_game, int _playerId, int rank, int suit)
{
	GiveCardsToGame(_game, _playerId, rank, suit);
}

static void ExchangeCardsCW(struct Game *_game)
{
	int suits[BANK_SIZE], ranks[BANK_SIZE], card1[2];
	int player, cardCnt, bankCnt;
	int rank, suit;
	
	bankCnt = 0;			
/*	select 3 highest ranked cards from each player (last three cards in array) and store in bank */
	for(player=0; player<VALID_NUM_PLAYERS; player++)
	{
		for(cardCnt = CARDS_EACH_HAND-1; cardCnt>=10; cardCnt--)
		{
			GetCard(_game, player, card1, PolicyGetCard, cardCnt);
			suits[bankCnt] = card1[0];
			ranks[bankCnt] = card1[1];
			bankCnt++;
		}
	}
/*	 Exchange 3 Cards among players */
	bankCnt = 0;
	for(player=1; player<VALID_NUM_PLAYERS; player++)
	{
			GiveCards(_game, player, ranks[bankCnt], suits[bankCnt]);
			bankCnt++;
			GiveCards(_game, player, ranks[bankCnt], suits[bankCnt]);
			bankCnt++;
			GiveCards(_game, player, ranks[bankCnt], suits[bankCnt]);
			bankCnt++;
	}
	/* handle the first player  */
	GiveCards(_game, 0, ranks[bankCnt], suits[bankCnt]);
	bankCnt++;
	GiveCards(_game, 0, ranks[bankCnt], suits[bankCnt]);
	bankCnt++;
	GiveCards(_game, 0, ranks[bankCnt], suits[bankCnt]);
	bankCnt++;
}

int FindPlayerViaRankSuit(struct Game *_game, int _rank, int _suit)
{
	int playerID;
	FindPlayerGame(_game, _rank, _suit, &playerID);
	return playerID;
}

int FindIdxOfCardViaRankSuit(struct Game *_game, int _rank, int _suit, int _player)
{
	int idx;
	FindIdxGame(_game, _rank, _suit, &idx, _player);
	return idx;
}

void FindImportantCards(struct Trick *_trick)
{
	int rank, suit, playerID;
	/* 2 of clubs */
	rank = TWO;
	suit = CLUBS;
	playerID = FindPlayerViaRankSuit(_trick->m_game, rank, suit);
	_trick->m_playerWClubsTwo = playerID;
	
	/* q of spades */
	rank = QUEEN;
	suit = SPADES;
	playerID = FindPlayerViaRankSuit(_trick->m_game, rank, suit);
	_trick->m_playerWSpadesQueen = playerID;
}

PassThreeCards(struct Game *_game)
{
	int player, cardCnt;
	SortHandsByRank(_game);	
	ExchangeCardsCW(_game);
	SortHands(_game);	
	

/*	 find player that holds 2 of cards & player w/ queen of spades */
/*	FindImportantCards(_trick);*/
}

void PrintTable(int *suits, int *ranks)
{
	int i;
	printf("------ TABLE: \n");
	for(i=0; i<4; i++)
	{
		printf("rank: %d suit: %d\n", ranks[i], suits[i]);
	}
}

void RunARound()
{
	struct Trick *newTrick;
	int card[2], IdxOfTwoClubs, i;
	int originalPlayer, turn;
	int suits[TABLE_SIZE], ranks[TABLE_SIZE];
	newTrick = CreateTrick(ZEROTH_TRICK);
	i=0;
	do
	{
		printf("-------- trick # : %d\n\n", newTrick->m_trickNumber);
		newTrick = CreateTrick(newTrick->m_trickNumber);
		PassThreeCards(newTrick->m_game);
		FindImportantCards(newTrick);
		PrintGameCards(newTrick->m_game);
		originalPlayer = newTrick->m_playerWClubsTwo;
		turn = newTrick->m_trickNumber == 0 ? newTrick->m_playerWClubsTwo : newTrick->m_startingPlayer;
		/* get card from first player, put it on the table, store this card */
		
		do
		{
			if(newTrick->m_trickNumber == 0)
			{
				printf("Leading Player: %d", turn);
				IdxOfTwoClubs = FindIdxOfCardViaRankSuit(newTrick->m_game, TWO, CLUBS, turn%VALID_NUM_PLAYERS);
				GetCard(newTrick->m_game, turn%VALID_NUM_PLAYERS, card, PolicyPutOnTable, IdxOfTwoClubs);
				suits[i] = card[0];
				ranks[i] = card[1];
				i++;
/*				nextPlayer = newTrick->m_playerWClubsTwo + 1;*/
				
			}
			else
			{
				/* prev player starts - the one who won last trick */
				
			}
			turn++;
			
		} while (turn % VALID_NUM_PLAYERS !=  originalPlayer);
		
		
		
		/* according to some policy, choose the next card to be put on the table, store this card */
		
		/* same idea for players 3,4 */
		
		/* player with highest ranked card "wins" the trick */
		
		
		newTrick->m_trickNumber++;
		
	} while(newTrick->m_trickNumber < 1);
	
	printf("AFTER: \n");
	PrintTable(suits, ranks);
/*	printf("cards :  %d %d \n\n", suits[0], ranks[0]);*/
/*	PrintGameCards(newTrick->m_game);*/
}
void SetUpGame(int _nBots, int _nHumans)
{
	RunARound();
	
	/* check status in each round */
/*	int r;*/
/*	r = CheckStatus(func1);*/
/*	printf("r %d", r);*/
	
	/* retreive scores for each players */
/*	CheckScores(func2);*/
/*	printf("\n");*/
}
/* END */











/*	sort each hand */
/*	PrintGameCards(newTrick->m_game);*/
/*	SortHands(newTrick->m_game);*/
/*	int card[2];*/
/*	PrintGameCards(newTrick->m_game);*/
/*	GetCard(newTrick->m_game, 0, card, PolicyGetCard, 11);*/
/*	PrintGameCards(newTrick->m_game);*/









