#ifndef __CARDS_H__
#define __CARDS_H__
#include "CardSpecs.h"
#include "SpecialChars.h"

struct Card;
size_t sizeOfCard;

/*
	desc: create a new card with suit and rank.
	args: 
		-_cardSuit: suit of the card (HEARTS, SPADES,DIAMONDS, CLUBS)
		-_cardRank: rank of card (two, three, ... , Jack, Queen, King, Ace)
	output: pointer to card struct.
	error handling:
		- returns NULL if malloc was not successful.
		
*/
struct Card  * CreateCard(Suit _cardSuit, Rank _cardRank);

/*
	desc: Get the Rank of card struct.
	args: 
		-_card: pointer to card struct.
	output: Rank (TWO,THREE,..., ACE) of card struct. 
	error handling:
		
*/
Rank GetRank(struct Card *_card);

/*
	desc: Get the suit of card struct.
	args: 
		-_card: pointer to card struct.
	output: Suit (HEARTS, SPADES, DIAMONDS, CLUBS) of card struct. 
	error handling:
		
*/
Suit GetSuit(struct Card *_card);

/*
	desc: Swap position of two cards. 
	args: 
		-_card1: 1st pointer to card struct.
		-_Card2: 2nd pointer to card struct. 
	output:
	error handling:
		
*/
void SwapCards(struct Card *_card1, struct Card *_card2);

/*
	desc: Free card struct.
	args: 
		-_card: pointer to card struct. 
	output:
	error handling:
		- retuns without doing any changes  if magic # was changed or if _card is a NULL pointer.
		
*/
void DestroyCard(struct Card *_card);

# endif /* __CARDS_H__ */
