/* possible NULL pointers */
#include <stdio.h>
/* malloc for a new cards */
#include <stdlib.h>
/* extract card struct + its functions + error signals + card specs */
#include "deck.h"

#define DECK_MAGIC_NUM 19931201

static void FillTheDeck(struct Deck *_deck, int _nCards)
{
	int cardCnt;
	Rank rank;
	Suit suit;
	
	cardCnt = 0;
	for(suit=HEARTS; suit<=CLUBS; suit++)
	{
		for(rank=TWO; rank<=ACE; rank++)
		{
			_deck->m_cards[cardCnt] =  CreateCard(suit, rank);
			cardCnt++;
			_deck->m_nCards++;
		}
	}
}

static void SwapCards(struct Card *_card1, struct Card *_card2)
{
	struct Card tmp;
	tmp = *_card1;
	*_card1 = *_card2;
	*_card2 = tmp;
}

void ShuffleTheDeck(struct Deck *_deck, int _nCards)
{
	size_t i, j;
	size_t suit, rank, cardCnt;
	for (i = _nCards-1; i > 0; i--)
    {
    	j = rand() % (i+1);
        SwapCards(_deck->m_cards[i], _deck->m_cards[j]);
    }
}

struct Deck * CreateDeck(size_t _nCards)
{
	struct Deck *newDeck;
	struct Card **newCards;
	int sizeOfCards;
	
/*	newDeck = (struct Deck*)malloc(sizeof(struct Deck));*/
	newDeck = (struct Deck*)calloc(1, sizeof(struct Deck));
	if(newDeck == NULL)
	{
		return NULL;
	}
	
/*	sizeOfCards = sizeof(struct Card) * _nCards;*/
/*	newCards = (struct Card**)malloc(sizeOfCards);*/
	newCards = (struct Card**)calloc(_nCards, sizeof(struct Card));
	if(newCards == NULL)
	{
		return NULL;
	}
	newDeck->m_cards = newCards; 
	newDeck->m_nCards = 0;
	newDeck->m_magic = DECK_MAGIC_NUM;
	
	FillTheDeck(newDeck, _nCards);
	ShuffleTheDeck(newDeck, _nCards);

	return newDeck;
}	

void DestroyDeck(struct Deck *_deck)
{
	int i;

	if(_deck == NULL || _deck->m_magic != DECK_MAGIC_NUM)
	{
		return;
	}
	_deck->m_magic = 0;
	for(i=0; i<_deck->m_nCards; i++)
	{
		free(_deck->m_cards[i]);
	}
	free(_deck);
}










