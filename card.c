#include <stdio.h>
/* malloc for a new cards */
#include <stdlib.h>
#define CARD_MAGIC_NUM 19931302
#include "card.h"
#include "SpecialChars.h"
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
	if(_card->m_suit == HEARTS)
	{
		printf("%s%s%d%s ", BOLD_ON, RED, _card->m_rank, HEART);
		printf(" ");
		printf("%s", WHITE);
		return;
	}
	if(_card->m_suit == SPADES)
	{
		printf("%s%s%d%s ", BOLD_ON, YELLOW, _card->m_rank, SPADE);
		printf(" ");
		printf("%s", WHITE);
		return;
	}
	if(_card->m_suit == CLUBS)
	{
		printf("%s%s%d %s ", BOLD_ON, BLUE, _card->m_rank, CLUB);
		printf(" ");
		printf("%s", WHITE);
		return;
	}
	if(_card->m_suit == DIAMONDS)
	{
		printf("%s%s%d %s ", BOLD_ON, CYAN, _card->m_rank, DIAMOND);
		printf(" ");
		printf("%s", WHITE);
		return;
	}
	
	
}

