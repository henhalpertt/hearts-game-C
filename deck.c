
/* possible NULL pointers */
#include <stdio.h>
/* malloc for a new cards */
#include <stdlib.h>
/* extract card struct + its functions + error signals + card specs */
#include "card.h"
/*#include "deck.h"*/
/*#include "deck.h"*/
#define DECK_MAGIC_NUM 19931201

struct Deck
{
	struct Card **m_cards;
	int m_nCards;
	int m_magic;
};


struct Card **GetCards(struct Deck *_deck)
{
	return _deck->m_cards;
}

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

static void ShuffleTheDeck(struct Deck *_deck, int _nCards)
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
	
	newDeck = (struct Deck*)calloc(1, sizeof(struct Deck));
	if(newDeck == NULL)
	{
		return NULL;
	}
	newCards = (struct Card**)calloc(_nCards, sizeOfCard);
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










