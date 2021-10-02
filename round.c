/* possible NULL pointers */
#include <stdio.h>
/* malloc for a new cards */
#include <stdlib.h>
/* delay for some parts of the game */
#include <time.h>
/* constants regarding hearts game such as # of cards for this game, # of cards for each player etc.. */
#include "PolicySignals.h"
#include "game.h"
#include "deck.h"
#define TRICK_MAGIC_NUM 1435654

struct Trick
{
	struct Game *m_game; /* communicating game status and scores */
	int *m_penalties; /* score of each player at the end of round */
	int ranks[TABLE_SIZE]; /* maximum of 4 cards on the table. holding ranks of cards */
	int suits[TABLE_SIZE]; /* maximum of 4 cards on the table. holding suits of cards */
	int m_trickNumber; /* 1-13 tricks */					
	int m_playerWClubsTwo; /* the player that is holding 2 of clubs ie starting the game */
	int m_playerWSpadesQueen; /* the player that is holding queen of spades */
	int m_leadingSuit; /* the leading suit for the trick */
	int m_turnNum; /* 4 turns per trick */
	int m_startingPlayer; /* player who "won" prev trick. unless its the 1st trick (playerWClubsTwo) */
	int m_heartsStatus; /* was hearts broken or is it still unbroken */
	int m_magic;
};

static struct Trick * CreateTrick(int _trickNum, int _nBots, int _nHumans)
{
	struct Trick *newTrick;
	newTrick = (struct Trick*)malloc(sizeof(struct Trick));
	if(newTrick == NULL)
	{
		return NULL;
	}
	newTrick->m_game = CreateGame(_nBots, _nHumans);
	
	newTrick->m_trickNumber = _trickNum;
	newTrick->m_playerWClubsTwo = INIT_MEMBER;
	newTrick->m_playerWSpadesQueen = INIT_MEMBER;
	newTrick->m_heartsStatus = UNBROKEN;
	newTrick->m_magic = TRICK_MAGIC_NUM;
	newTrick->m_leadingSuit = INIT_MEMBER;
	newTrick->m_startingPlayer = INIT_MEMBER;
	newTrick->m_turnNum = 0;
	newTrick->m_penalties = NULL;
	return newTrick;
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
/* PTR to function - implement here */
int PolicyGetCard(int _cardIdx)
{
	/* return the  card idx  and to get rid of the cards */
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

static void ExchangeCardsCW(struct Game *_game, int *_botOrHuman)
{
	int suits[BANK_SIZE], ranks[BANK_SIZE], card1[CARD_SIZE];
	int player, cardCnt, bankCnt;
	int rank, suit;
	int humanInput=0;
	int flg;
	int playerIdx;
	
	bankCnt = 0;			
/*	select 3 highest ranked cards from each player (last three cards in array) and store in bank */
	for(player=0; player<VALID_NUM_PLAYERS; player++)
	{
		if(_botOrHuman[player] == BOT)
		{
			for(cardCnt = CARDS_EACH_HAND-1; cardCnt>=10; cardCnt--)
			{
				GetCard(_game, player, card1, PolicyGetCard, cardCnt);
				suits[bankCnt] = card1[0];
				ranks[bankCnt] = card1[1];
				bankCnt++;
			}
		}
		else
		{
			PrintStrUI(PASS_THREE_CARDS);
			SortHands(_game);
			flg=0;
			while(flg  != 3)
			{
				PrintStrUI(YOUR_CARDS);
				PrintHand(_game, player);
				PrintStrUI(ENTER_CARDIDX);
				UserInput(&playerIdx);
				if(playerIdx < 0 || playerIdx > BANK_SIZE )
				{
					PrintStrUI(IDX_BOUNDS);	
				}
				else
				{
					GetCard(_game, player, card1, PolicyGetCard, playerIdx);
					suits[bankCnt] = card1[0];
					ranks[bankCnt] = card1[1];
					bankCnt++;
					flg++;	
				}
			}
			SortHandsByRank(_game);	
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

PassThreeCards(struct Game *_game, int *_botOrHuman)
{
	int player, cardCnt;
	SortHandsByRank(_game);	
	ExchangeCardsCW(_game, _botOrHuman);
	SortHands(_game);
}

int FindWinner(int *_ranks, int *_suits, int _origPlayer, int *_heartCnt, int _leadSuit)
{
	int tmp=0, maxIdx, player;
	int hrtcnt=0;
	for(player=0; player<4; player++)
	{
		if(_ranks[player] > tmp && _suits[player] == _leadSuit)
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

void CallPlayerOne(struct Trick *_trick , int *_card, int _turn)
{
	int playerIdx, result;
	int flg=0;
	PrintStrUI(YOUR_CARDS);
	PrintHand(_trick->m_game, _turn);
	while(flg==0)
	{
		PrintStrUI(ENTER_CARDIDX);
		UserInput(&playerIdx);
/*		scanf("%d", &playerIdx);*/
		SeeCardGame(_trick->m_game, _turn, _card, playerIdx);
		/* check if idx is valid */
		if(playerIdx < 0 || playerIdx > CARD_IDX_LIMIT - _trick->m_trickNumber )
		{
			PrintStrUI(IDX_BOUNDS);
		}
		else if(_trick->m_heartsStatus != BROKEN && _card[0] == HEARTS) 
		{
			PrintStrUI(HEART_SUIT_ERR);
		}
		else
		{
			GetCard(_trick->m_game, _turn, _card, PolicyGetCard, playerIdx);
			flg = 1;
		}
	}
}

void CallPlayer(struct Trick *_trick , int *_card, int _turn, int _leadSuit)
{
	int playerIdx, result;
	int flg=0;
	result = CheckSuitGame(_trick->m_game, _turn, _leadSuit);
	PrintStrUI(HUMAN_CARDS);
	PrintHand(_trick->m_game, _turn);
	while(1)
	{
		PrintStrUI(ENTER_CARDIDX);
		UserInput(&playerIdx);
/*		scanf("%d", &playerIdx);*/
		SeeCardGame(_trick->m_game, _turn, _card, playerIdx);
		/* check if idx is valid */
		if(playerIdx < 0 || playerIdx > CARD_IDX_LIMIT - _trick->m_trickNumber)
		{
			PrintStrUI(IDX_BOUNDS);
		}
		else if(_leadSuit != _card[0])
		{
			if(result == SUIT_AVAILABLE)
			{
				SuitNumToSuitNameUI(_leadSuit);
				PrintStrUI(WRONG_SUIT);
				continue;
			}
			if(result == SUIT_NOT_FOUND)
			{
				if(_card[0] == HEARTS)
				{
					if(_trick->m_heartsStatus != BROKEN)
					{
						_trick->m_heartsStatus = BROKEN;
						printf("----->2\n");
						PrintStrUI(HEARTS_BROKEN);
					}
				}
			}
			GetCard(_trick->m_game, _turn, _card, PolicyGetCard, playerIdx);
			return;
		}
		else
		{
			/* same suit */
			GetCard(_trick->m_game, _turn, _card, PolicyGetCard, playerIdx);
			return;
		}
		
	}
}


int FindBestCard(struct Trick *_trick, int _playerID, int _leadRank, int _status)
{
	int idx;
	FindBestCardGame(_trick->m_game, _playerID, _trick->m_leadingSuit, _leadRank,  &idx, _status);
	return idx;
}


void CallPlayerBot(struct Trick *_trick , int *_card, int _turn, int _leadSuit)
{
	int playerIdx, result;
	int flg=0;
	int cards[CARDS_EACH_HAND];
	PrintStrUI(BOT_CARDS);
	PrintHand(_trick->m_game, _turn);
	
	playerIdx = FindBestCard(_trick, _turn, _card[1], _trick->m_heartsStatus);
	GetCard(_trick->m_game, _turn, _card, PolicyGetCard, playerIdx);
}

void RunARound(struct Trick *newTrick, int *_botOrHuman)
{
	int *m;
	int card[CARD_SIZE];
	int suits[TABLE_SIZE];
	int ranks[TABLE_SIZE];
	int roundScores[TABLE_SIZE] = {0};
	
	int IdxOfTwoClubs, i, bestIdx, tmpIdx;
	int originalPlayer, turn;
	int tmpRank = 0;
	int curWinner;
	int heartCnt;
	int result;
	int score;
	
	PassThreeCards(newTrick->m_game, _botOrHuman);
	FindImportantCards(newTrick);
	turn = newTrick->m_playerWClubsTwo;
	newTrick->m_leadingSuit = CLUBS;
	i=0;
	do
	{
		SortHands(newTrick->m_game);
		PrintValueUI(TRICK_NUM, newTrick->m_trickNumber);
		originalPlayer = turn;
		if(newTrick->m_turnNum == 0)
		{
			PrintHand(newTrick->m_game, turn%VALID_NUM_PLAYERS);
			if(newTrick->m_trickNumber == 0)
			{
				IdxOfTwoClubs = FindIdxOfCardViaRankSuit(newTrick->m_game, TWO, CLUBS, turn%VALID_NUM_PLAYERS);
				GetCard(newTrick->m_game, turn%VALID_NUM_PLAYERS, card, PolicyGetCard, IdxOfTwoClubs);
			}
			else
			{	
				if(_botOrHuman[turn%VALID_NUM_PLAYERS] == BOT)
				{
					tmpIdx = CARD_IDX_LIMIT-newTrick->m_trickNumber;
					SeeCardGame(newTrick->m_game, turn%VALID_NUM_PLAYERS, card, tmpIdx);
					/* keep queen of spades for players 2,3,4 */ 
					if(card[1] == QUEEN && card[0] == SPADES && tmpIdx-1 >=  0)
					{
						tmpIdx = tmpIdx-1;
						SeeCardGame(newTrick->m_game, turn%VALID_NUM_PLAYERS, card, tmpIdx);
						if(card[0] == HEARTS && newTrick->m_heartsStatus != BROKEN)
						{
						/* special case where I only have queen of spades, rest are hearts, but hearts status unbroken*/
							tmpIdx += 1;
						}
					}
					GetCard(newTrick->m_game, turn%VALID_NUM_PLAYERS, card, PolicyGetCard, tmpIdx);
				}
				else
				{
					CallPlayerOne(newTrick, card, turn%VALID_NUM_PLAYERS);
				}
				newTrick->m_leadingSuit = card[0];
			}
			newTrick->suits[i] = card[0];
			newTrick->ranks[i] = card[1];
			/* check hearts status */
			if(newTrick->suits[i] == HEARTS && newTrick->m_heartsStatus != BROKEN)
			{
				printf("----->3\n");
				PrintStrUI(HEARTS_BROKEN);
				newTrick->m_heartsStatus = BROKEN;
			}
			PrintTableCard(turn%VALID_NUM_PLAYERS, newTrick->ranks[i], newTrick->suits[i]);
			i++;
			turn++;
			newTrick->m_turnNum++;
			sleep(1);
		}

		/* 2,3,4 players */
		while (turn % VALID_NUM_PLAYERS !=  originalPlayer)
		{
			if(_botOrHuman[turn%VALID_NUM_PLAYERS] == BOT)
			{
				CallPlayerBot(newTrick, card, turn%VALID_NUM_PLAYERS, newTrick->m_leadingSuit);
			}
			else
			{
				CallPlayer(newTrick, card, turn%VALID_NUM_PLAYERS, newTrick->m_leadingSuit);
			}
			newTrick->suits[i] = card[0];
			newTrick->ranks[i] = card[1];
			if(newTrick->suits[i] == HEARTS && newTrick->m_heartsStatus != BROKEN)
			{
				printf("----->1\n");
				PrintStrUI(HEARTS_BROKEN);
				newTrick->m_heartsStatus = BROKEN;
			}
			PrintTableCard(turn%VALID_NUM_PLAYERS, newTrick->ranks[i], newTrick->suits[i]);
			i++;
			turn++;
			newTrick->m_turnNum++;
			sleep(1);
		}	
		/* player with highest ranked card "wins" the trick */		
		curWinner = FindWinner(newTrick->ranks, newTrick->suits, originalPlayer,
																	 &heartCnt, newTrick->m_leadingSuit);
		roundScores[curWinner] = roundScores[curWinner] + heartCnt;
		PrintScores(roundScores, VALID_NUM_PLAYERS);
		newTrick->m_turnNum = 0;
		newTrick->m_trickNumber++;
		/* loser starts next round */
		turn=curWinner;
		heartCnt = 0;
		i=0;
	} while(newTrick->m_trickNumber < CARDS_EACH_HAND);
	
	PrintStrUI(END_RND);
	/* communicate scores back to game */
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

static void DestroyTrick(struct Trick *_trick)
{
	if(_trick == NULL || _trick->m_magic != TRICK_MAGIC_NUM)
	{
		return;
	}
	_trick->m_magic = 0;
	/* game struct was already destroyed at this point */
	free(_trick->m_penalties);
	free(_trick);
}

void SetUpGame(int _nPlayers)
{
	int flg;
	int bots, humans;
	int b, p;
	int realOrNot[VALID_NUM_PLAYERS];
	
	struct Trick *trick1;
	PrintStrUI(TITLE);
	/*	srand(time(NULL));*/
	if(_nPlayers > VALID_NUM_PLAYERS || _nPlayers < 0)
	{
		PrintStrUI(ERR_NUM_PLAYERS);
		return;
	}
	bots = VALID_NUM_PLAYERS - _nPlayers;
	humans = VALID_NUM_PLAYERS - bots;
	for(b=0; b<bots; b++)
	{
		realOrNot[b] = BOT;
	}
	for(p=bots; p< (humans+bots); p++)
	{
		realOrNot[p] = HUMAN;
	}
	trick1 = CreateTrick(ZEROTH_TRICK, bots, humans);
	PrintStrUI(START_RND);
	while(flg != GAME_OVER)
	{
		RunARound(trick1, realOrNot);
		PrintScores(trick1->m_penalties, VALID_NUM_PLAYERS);
		flg = UpdateScores(trick1->m_game, trick1->m_penalties, GameStatus);
		RefillDeck(trick1->m_game);
/*		sleep(10);*/
	}
	DestroyTrick(trick1);
}
/* END */









