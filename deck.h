#ifndef __CARDS_H__
#define __CARDS_H__
#include "Errors.h"
#include "CardSpecs.h"
#include "card.h"

struct Deck
{
	struct Card **m_cards;
	int m_nCards;
	int m_magic;
};

struct Card
{
	Rank m_suit;
	Suit m_rank;
	int m_magic;
};

struct Deck * CreateDeck(size_t _nCards);
struct Card  * CreateCard(Suit cardSuit, Rank cardRank);

# endif /* __CARDS_H__ */
