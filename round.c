/* possible NULL pointers */
#include <stdio.h>
/* malloc for a new cards */
#include <stdlib.h>
#define TRICK_MAGIC_NUM 1435654
#define ROUND_MAGIC_NUM 2334543
#define ROUNDS_MAGIC_NUM 2983643
#define FIRST_TRICK 1
#define CARDS_EACH_HAND 13
#define BANK_SIZE 12

#include "deck.h"
#include "card.h"
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
struct Hand
{
	struct Card **m_cards; /* a subset of cards from deck (13 cards initially)*/
	int m_nCardsInHand; /* 13, after each trick: -1 */
	int m_playerID;
};

struct Trick
{
	struct Team *m_team;
	struct Deck *m_deck;
	struct Game *m_game;
	struct Hand **m_hands;
	struct Card **m_bank; /* for exchanging cards among player - stores 3 cards from each player*/
	
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


static void FillHand(struct Trick *_trick, struct Hand *_hand, int _from)
{
	int card;
	Rank rank;
	Suit suit;
	
	for(card= _from; card < _from+CARDS_EACH_HAND; card++)
	{
		
		rank = _trick->m_deck->m_cards[card]->m_rank;
		suit = _trick->m_deck->m_cards[card]->m_suit;
		_hand->m_cards[card - _from] = CreateCard(suit, rank);
	}
}

static struct Hand * CreateHand(int _playerID)
{
	struct Hand *newHand;
	struct Card **newCardsForPlayer;
	
	int from;
	
	newHand = (struct Hand*)malloc(sizeof(struct Hand));
	if(newHand == NULL)
	{
		return NULL;
	}
	newCardsForPlayer = (struct Card**)malloc(sizeof(struct Card) * CARDS_EACH_HAND);
	if(newCardsForPlayer == NULL)
	{
		return NULL;
	}
	
	from = CARDS_EACH_HAND * (_playerID);  /* from: [0-13), [13-26), [26-39), [39-52) */
	newHand->m_nCardsInHand = CARDS_EACH_HAND;
	newHand->m_cards = newCardsForPlayer;
	newHand->m_playerID = _playerID;
	
	return newHand;
}


static void SplitCardsToPlayers(struct Trick *_trick)
{	
	struct Hand *hand1= NULL;
	struct Hand *hand2= NULL;
	struct Hand *hand3= NULL;
	struct Hand *hand4= NULL;
	struct Hands *hands;
	struct Hand **newHands;
	
	int player,card, nCards, nPlayers, from;
	if(_trick == 0)
	{
		return;
	}
	nCards = _trick->m_deck->m_nCards;
	nPlayers = _trick->m_team->m_totalPlayers;
	if(nCards != NUM_CARDS || nPlayers != VALID_NUM_PLAYERS)
	{
		printf("# of cards or players invalid");
		return;
	}

	newHands = (struct Hand**)malloc(sizeof(struct Hand) * 4);
	if(newHands == NULL)
	{
		return;
	}
	
	_trick->m_hands = newHands;
	
	for(player=0, from=0; player<4; player++, from+=13)
	{
		_trick->m_hands[player] = CreateHand(player); /* new hand */ 
		FillHand(_trick, _trick->m_hands[player], from);
	}
	
	return;
}

static void SwapCards(struct Card *_card1, struct Card *_card2)
{
	struct Card tmp;
	tmp = *_card1;
	*_card1 = *_card2;
	*_card2 = tmp;
}

static int FindMinInSubset(struct Hand *_hand, size_t _idx)
{
	int tmp, suitItem, rankItem, item;
	size_t minIdx=_idx;
	
	tmp = 100*_hand->m_cards[_idx]->m_suit + _hand->m_cards[_idx]->m_rank;
	for(;_idx<CARDS_EACH_HAND; _idx++)
	{
		suitItem = 100 * (_hand->m_cards[_idx]->m_suit);
		rankItem = _hand->m_cards[_idx]->m_rank;
		item = suitItem + rankItem;

		if(tmp > item)
		{
			minIdx = _idx;
			tmp = item;
		}
	}
	return minIdx;
}

static int FindMinInSubsetByRank(struct Hand *_hand, size_t _idx)
{
	int tmp, rankItem;
	size_t minIdx=_idx;
	
	tmp = _hand->m_cards[_idx]->m_rank;
	for(;_idx<CARDS_EACH_HAND; _idx++)
	{
		rankItem = _hand->m_cards[_idx]->m_rank;
		if(tmp > rankItem)
		{
			minIdx = _idx;
			tmp = rankItem;
		}
	}
	return minIdx;
}

static void SortEachHand(struct Hand *_hand)
{
	size_t card, minIdx;
	int item;
	
	/* for every card in hand */
	for(card=0; card<CARDS_EACH_HAND-1; card++)
	{
		/* selection sort */
		minIdx = FindMinInSubset(_hand, card);
		SwapCards(_hand->m_cards[minIdx], _hand->m_cards[card]);
	}
}

static void SortEachHandByRank(struct Hand *_hand)
{
	size_t card, minIdx;
	int item;
	
	/* for every card in hand */
	for(card=0; card<CARDS_EACH_HAND-1; card++)
	{
		/* selection sort */
		minIdx = FindMinInSubsetByRank(_hand, card);
		SwapCards(_hand->m_cards[minIdx], _hand->m_cards[card]);
	}
}

static void ExchangeCardsCW(struct Trick *_trick)
{
	struct Card **bank;
	int player, cardCnt, bankCnt;
	
	bank = (struct Card**)malloc(sizeof(struct Card) * BANK_SIZE); /* 3 cards from each player */
	if(bank == NULL)
	{
		return;
	}
	
	_trick->m_bank = bank;
	bankCnt = 0;
	/*select 3 highest ranked cards from each player (last three cards in array) and store in bank */
	for(player=0; player<VALID_NUM_PLAYERS; player++)
	{
		for(cardCnt = 10; cardCnt<CARDS_EACH_HAND; cardCnt++)
		{
			_trick->m_bank[bankCnt] = _trick->m_hands[player]->m_cards[cardCnt];
			bankCnt++;
		}
	}
	/* Exchange 3 Cards among players */
	bankCnt = 0;
	for(player=1; player<VALID_NUM_PLAYERS; player++)
	{
			_trick->m_hands[player]->m_cards[10] = _trick->m_bank[bankCnt++];
			_trick->m_hands[player]->m_cards[11] = _trick->m_bank[bankCnt++];
			_trick->m_hands[player]->m_cards[12] = _trick->m_bank[bankCnt++];
	}
	/* handle the first player */
	_trick->m_hands[0]->m_cards[10] = _trick->m_bank[bankCnt++];
	_trick->m_hands[0]->m_cards[11] = _trick->m_bank[bankCnt++];
	_trick->m_hands[0]->m_cards[12] = _trick->m_bank[bankCnt++];

}

PassThreeCards(struct Trick *_trick)
{
/*	
	Typically you want to pass your three worst cards (3 highest ranked cards in this policy).
	 Which opponent you pass to varies, you start by passing to the opponent on your left,
	  then in the next game you pass to the opponent on your right,
	   third game you pass across the table and in the fourth game there is no 	card passing.
*/
	int player, cardCnt;
	for(player=0; player<VALID_NUM_PLAYERS; player++)
	{
		SortEachHandByRank(_trick->m_hands[player]);
	}	
	
	ExchangeCardsCW(_trick);
	
	/* re-sort according to game policy */
	for(player=0; player<VALID_NUM_PLAYERS; player++)
	{
		SortEachHand(_trick->m_hands[player]);
	}
}
void SetUpGame(struct Input *_in)
{
	int nCards, nBots, nHumans, totalPlayers;
	int suit, rank, cardCnt;
	int player; /*idx */
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
	/* Create players */
	newTeam = MakePlayers(nBots, nHumans);
	totalPlayers = newTeam->m_totalPlayers;
	/*Create a Game*/
	newGame = CreateGame(totalPlayers);

	input.m_players = newTeam;
	input.m_cards = newDeck;
	input.m_game = newGame;
	
	firstTrick = CreateTrick(FIRST_TRICK, input);	
	SplitCardsToPlayers(firstTrick);
	
	/*sort each hand */
	for(player=0; player<VALID_NUM_PLAYERS; player++)
	{
		SortEachHand(firstTrick->m_hands[player]);
	}
	
	for(player=0; player<VALID_NUM_PLAYERS; player++)
	{
		printf("\nBEFORE ID: %d:\n", firstTrick->m_hands[player]->m_playerID);
		for(cardCnt = 0; cardCnt<CARDS_EACH_HAND; cardCnt++)
		{
			printf("suit: %d rank: %d \n\n", firstTrick->m_hands[player]->m_cards[cardCnt]->m_suit,
										 firstTrick->m_hands[player]->m_cards[cardCnt]->m_rank);
		}
		printf("\n");
	}
	
	printf("AFTER passing 3 cards\n\n");
	/* pass 3 cards */
	PassThreeCards(firstTrick);
	
	for(player=0; player<VALID_NUM_PLAYERS; player++)
	{
		printf("\nAFTER ID: %d:\n", firstTrick->m_hands[player]->m_playerID);
		for(cardCnt = 0; cardCnt<CARDS_EACH_HAND; cardCnt++)
		{
			printf("suit: %d rank: %d \n\n", firstTrick->m_hands[player]->m_cards[cardCnt]->m_suit,
										 firstTrick->m_hands[player]->m_cards[cardCnt]->m_rank);
		}
		printf("\n");
	}
	
	printf("\n");
}

/* END */





































/*
print cards 
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


players
	for(player=0; player<totalPlayers; player++)
	{
		printf("\nplayer id: %d, real? %d, inOrOut? %d, winner? %d \n",
					newTeam->m_players[player]->m_id,
					newTeam->m_players[player]->m_realOrNot,
					newTeam->m_players[player]->m_inOrOutOfTheGame,
					newTeam->m_players[player]->m_winner);
	}

game

	for(player=0; player<totalPlayers; player++)
	{
		printf("\nplayer initial score: %d\n", newGame->m_scores[player]);
	}


*/






















