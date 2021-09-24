#ifndef __CARDS_H__
#define __CARDS_H__
#include "Errors.h"
#include "CardSpecs.h"

struct Card
{
	Rank m_suit;
	Suit m_rank;
	int m_magic;
};

/*
	desc: create a new card with suit and rank.
	args: 
		-_cardSuit: suit of the card (HEARTS, SPADES,DIAMONDS, CLUBS)
		-_cardRank: rank of card (two, three, ... , Jack, Queen, King, Ace)
	output: pointer to card.
	error handling:
		- ERR_NULL_PTR if malloc was not successful.
		
*/
struct Card  * CreateCard(Suit cardSuit, Rank cardRank);

void DestroyCard(struct Card *_card);

# endif /* __CARDS_H__ */
