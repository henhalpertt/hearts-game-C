#include <stdio.h>
/* malloc for a new cards */
#include <stdlib.h>
#define CARD_MAGIC_NUM 19931302
#include "card.h"

struct Card
{
	Rank m_suit;
	Suit m_rank;
	int m_magic;
};

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

void SwapCards(struct Card *_card1, struct Card *_card2)
{
	struct Card tmp;
	tmp = *_card1;
	*_card1 = *_card2;
	*_card2 = tmp;
}

int GetRank(struct Card *_card)
{
	return _card->m_rank;
}

int GetSuit(struct Card *_card)
{
	return _card->m_suit;
}

void PrintCard(struct Card *_card)
{
	printf("rank:%d suit:%d", _card->m_rank, _card->m_suit);
}

