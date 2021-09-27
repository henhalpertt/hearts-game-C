#ifndef __CARDS_H__
#define __CARDS_H__
#include "Errors.h"
#include "CardSpecs.h"

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

int GetRank(struct Card *_card);
int GetSuit(struct Card *_card);
void SwapCards(struct Card *_card1, struct Card *_card2);
void DestroyCard(struct Card *_card);

void PrintCard(struct Card *_card);

# endif /* __CARDS_H__ */
