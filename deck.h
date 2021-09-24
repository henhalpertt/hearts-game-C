#ifndef __CARDS_H__
#define __CARDS_H__
#include <stdlib.h>
#include "Errors.h"
#include "CardSpecs.h"
#include "card.h"

typedef struct Deck Deck;
typedef struct Card Card;

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

# endif /* __CARDS_H__ */
