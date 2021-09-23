/* possible NULL pointers */
#include <stdio.h>
/* malloc for a new cards */
#include <stdlib.h>
/* extract card struct + its functions + error signals + card specs */
/*#include "card.h"*/
/*#include "Errors.h"*/
/*#include "CardSpecs.h"*/
#include "card.h"

struct Deck
{
	struct Card **m_cards;
	size_t m_nCards;
	int m_magic;
};

static void FillTheDeck(struct Deck *_deck, int _nCards)
{
	size_t suit, rank, cardCnt;
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

static void ShuffleTheDeck(struct Deck *_deck, int _nCards)
{
	size_t i, j;
	size_t suit, rank, cardCnt;
	srand(time(NULL));
	for (i = _nCards-1; i > 0; i--)
    {
    	j = rand() % (i+1);
        SwapCards(_deck->m_cards[i], _deck->m_cards[j]);
    }
}

/* _nCards - # of cards  */
struct Deck * CreateDeck(int _nCards)
{
	struct Deck *newDeck;
	struct Card **newCards;
	int sizeOfCards;
	size_t suit, rank, cardCnt;
	newDeck = (struct Deck*)malloc(sizeof(struct Deck));
	if(newDeck == NULL)
	{
		return NULL;
	}
	
	sizeOfCards = sizeof(struct Card) * _nCards;
	newCards = (struct Card**)malloc(sizeOfCards);
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

int main(void)
{
	struct Deck *newDeck;
	size_t suit, rank, cardCnt;
	newDeck = CreateDeck(52);
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
	return 0;
	
}








