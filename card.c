#include <stdio.h>
/* malloc for a new cards */
#include <stdlib.h>
#define CARD_MAGIC_NUM 19931302
/* including some card specs */
#include "card.h"
struct Card
{
	Rank m_suit;
	Suit m_rank;
	int m_magic;
};

size_t sizeOfCard = sizeof(struct Card);

struct Card * CreateCard(Suit _cardSuit, Rank _cardRank)
{
	struct Card *newCard;
	newCard = (struct Card*)malloc(sizeof(struct Card));
	if(newCard == NULL)
	{
		return NULL;
	}
	newCard->m_suit = _cardSuit;
	newCard->m_rank = _cardRank;
	newCard->m_magic = CARD_MAGIC_NUM;
	return newCard;
}

void DestroyCard(struct Card *_card)
{
	if(_card == NULL || _card->m_magic != CARD_MAGIC_NUM)
	{
		return;
	}
	_card->m_magic = 0;
	free(_card);
}

void SwapCards(struct Card *_card1, struct Card *_card2)
{
	struct Card tmp;
	tmp = *_card1;
	*_card1 = *_card2;
	*_card2 = tmp;
}

Rank GetRank(struct Card *_card)
{
	return _card->m_rank;
}

Suit GetSuit(struct Card *_card)
{
	return _card->m_suit;
}


