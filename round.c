/* possible NULL pointers */
#include <stdio.h>
/* malloc for a new cards */
#include <stdlib.h>
#include <string.h>
#include <time.h>
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
#define QSPADES_PENALTY 13
#define GAME_OVER 100
#define RUNNING_GAME 1

#include "game.h"
#include "round.h"
#include "deck.h"
#include "card.h"
#include "player.h"

struct Trick
{
	struct Game *m_game; /* communicating game status and scores */
	int *m_penalties; /* score of each player at the end of round */
	int ranks[4]; 
	int suits[4];
	int m_trickNumber; /* 1-13 tricks */					
	int m_playerWClubsTwo; /* the player that is holding 2 of clubs ie starting the game */
	int m_playerWSpadesQueen; /* the player that is holding queen of spades */
	int m_leadingSuit; /* the leading suit for the trick */
	int m_turnNum; /* 4 turns per trick */
	int m_startingPlayer; /* starting player = player who "won" prev trick. unless its the 1st trick (playerWClubsTwo) */
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
	newTrick->m_turnNum = 0;
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
	ShuffleTheDeck(newDeck, _nCards);
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
/*	if(_heartsStat == BROKEN)*/
/*	{*/
/*		*/
/*	}*/
	return _cardIdx;
}

PolicyFindLowestCard(struct Trick *_trick)
{
	/* check cards on table */
	/* Tell player to put a card in that suit, and with rank lower then what is currently the lowest, if possible */
	
	/* scenario 1: player doesnt have cards from suit */
	/* scenario 2: player has cards from same suit, then put smallest one you got in hand */ 
	
	
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
}

int FindBestCard(struct Trick *_trick, int _playerID, int _leadRank)
{
	int idx;
	FindBestCardGame(_trick->m_game, _playerID, _trick->m_leadingSuit, _leadRank,  &idx);
	return idx;
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


int FindWinner(int *_ranks, int *_suits, int _origPlayer, int *_heartCnt)
{
	int tmp=0, maxIdx, player;
	int hrtcnt=0;
	for(player=0; player<4; player++)
	{
		if(_ranks[player] > tmp)
		{
			maxIdx = player;
			tmp = _ranks[player];
		}
		if(_suits[player] == HEARTS)
		{
			hrtcnt++;
		}
		if(_ranks[player] == QUEEN && _suits[player] == SPADES)
		{
			hrtcnt += QSPADES_PENALTY;
		}
	}
	*_heartCnt = hrtcnt;
	return (maxIdx + _origPlayer) % VALID_NUM_PLAYERS;
}


void PrintScores(int *_scores)
{
	int nPlayers, score;
	for(score=0; score<VALID_NUM_PLAYERS; score++)
	{
		printf("player: %d Score: %d \n", score, _scores[score]);
	}
}

void RunARound(struct Trick *newTrick)
{
	int card[2], IdxOfTwoClubs, i;
	int originalPlayer, turn;
	int suits[TABLE_SIZE], ranks[TABLE_SIZE], bestIdx;
	int tmpRank = 0, curWinner;
	int roundScores[4] = {0}, heartCnt;
	PassThreeCards(newTrick->m_game);
	FindImportantCards(newTrick);
	turn = newTrick->m_playerWClubsTwo;
	i=0;
	do
	{
		printf("-------- trick # : %d\n", newTrick->m_trickNumber);
/*		turn = newTrick->m_trickNumber == 0 ? newTrick->m_playerWClubsTwo : newTrick->m_startingPlayer;*/
		originalPlayer = turn;
		if(newTrick->m_turnNum == 0)
		{
			if(newTrick->m_trickNumber == 0)
			{
				IdxOfTwoClubs = FindIdxOfCardViaRankSuit(newTrick->m_game, TWO, CLUBS, turn%VALID_NUM_PLAYERS);
				GetCard(newTrick->m_game, turn%VALID_NUM_PLAYERS, card, PolicyGetCard, IdxOfTwoClubs);
				newTrick->m_leadingSuit = CLUBS;
				newTrick->suits[i] = card[0];
				newTrick->ranks[i] = card[1];
			}
			else
			{	
				GetCard(newTrick->m_game, turn%VALID_NUM_PLAYERS, card, PolicyGetCard, 12-newTrick->m_trickNumber);
				newTrick->suits[i] = card[0];
				newTrick->ranks[i] = card[1];
				newTrick->m_leadingSuit = card[0];
			}
			/* check hearts status */
			if(newTrick->suits[i] == HEARTS && newTrick->m_heartsStatus != BROKEN)
			{
				printf("HEARTS STATUS BROKEN \n");
				newTrick->m_heartsStatus = BROKEN;
			}
			i++;
			turn++;
			newTrick->m_turnNum++;
		}

		/* 2,3,4 players */
		while (turn % VALID_NUM_PLAYERS !=  originalPlayer)
		{
			bestIdx = FindBestCard(newTrick, turn%VALID_NUM_PLAYERS, card[1]);
			GetCard(newTrick->m_game, turn%VALID_NUM_PLAYERS, card, PolicyGetCard, bestIdx);
			
			newTrick->suits[i] = card[0];
			newTrick->ranks[i] = card[1];
			if(newTrick->suits[i] == HEARTS && newTrick->m_heartsStatus != BROKEN)
			{
				printf("HEARTS STATUS BROKEN \n");
				newTrick->m_heartsStatus = BROKEN;
			}
			i++;
			turn++;
			newTrick->m_turnNum++;
		}
		 	
		/* player with highest ranked card "wins" the trick */		
		curWinner = FindWinner(newTrick->ranks, newTrick->suits, originalPlayer, &heartCnt);
		roundScores[curWinner] = roundScores[curWinner] + heartCnt;
		printf("fds");
		PrintScores(roundScores);
		
		newTrick->m_turnNum = 0;
		newTrick->m_trickNumber++;
		/* loser starts next round */
		turn=curWinner;
		heartCnt = 0;
		i=0;
		
	} while(newTrick->m_trickNumber < 13);
	
	printf("end---\n");
	/* communicate scores back to game */
	int *m, score;
	m = (int *)calloc(1,sizeof(roundScores));
	newTrick->m_penalties = m;
	
	for(score=0; score<VALID_NUM_PLAYERS; score++)
	{
		newTrick->m_penalties[score] = roundScores[score];
	}
	newTrick->m_trickNumber = 0;
	newTrick->m_heartsStatus = UNBROKEN;
	newTrick->m_turnNum = 0;
	return;
}

int GameStatus(int *_scores)
{
	int score;
	for(score=0; score<VALID_NUM_PLAYERS; score++)
	{
		if(_scores[score] >= 100)
		{
			return GAME_OVER;
		}
	}
	return RUNNING_GAME;
}



void SetUpGame(int _nBots, int _nHumans)
{
	int flg;
	int rnd;
	srand(time(NULL));
	struct Trick *trick1;

	trick1 = CreateTrick(ZEROTH_TRICK);
	rnd = 1;
	while(flg != GAME_OVER)
	{
		printf("***** Round %d ****** \n", rnd);
		RunARound(trick1);
/*		PrintScores(trick1->m_penalties);*/
		flg = UpdateScores(trick1->m_game, trick1->m_penalties, GameStatus);
		RefillDeck(trick1->m_game);
/*		sleep(1);*/
		rnd++;
	}
	
	/* check status in each round */
/*	int r;*/
/*	r = CheckStatus(func1);*/
/*	printf("r %d", r);*/
	
	/* retreive scores for each players */
/*	CheckScores(func2);*/
/*	printf("\n");*/
}
/* END */









