/* catching errors */
/*#include "Errors.h"*/
/* possible suit and rank of card */
/*#include "CardSpecs.h"*/
/* possible NULL pointers */
#include <stdio.h>
/* malloc for a new cards */
#include <stdlib.h>
#define CARD_MAGIC_NUM 19931302
#include "card.h"

struct Card * CreateCard(Suit cardSuit, Rank cardRank)
{
	struct Card *newCard;
	newCard = (struct Card*)malloc(sizeof(struct Card));
	if(newCard == NULL)
	{
		return NULL;
	}
	newCard->m_suit = cardSuit;
	newCard->m_rank = cardRank;
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


