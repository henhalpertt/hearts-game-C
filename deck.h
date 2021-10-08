#ifndef __CARDS_H__
#define __CARDS_H__
/*#include "CardSpecs.h"*/
/*#include "card.h"*/

struct Deck;
struct Card;

/*
	desc: create a new Deck of cards with cards from struct Card.
	args: 
		-_nCards: # of cards. 
	output: pointer to Deck struct.
	error handling:
		- returns NULL if calloc of new deck was not successful.
		- returns NULL if calloc for new cards was not successful.
*/
struct Deck * CreateDeck(size_t _nCards);

/*void ShuffleTheDeck(struct Deck *_deck, int _nCards);*/

/*
	desc: Free Deck struct and all the cards structures in the deck.
	args: 
		-_deck: pointer to Deck struct. 
	output: pointer to Deck struct.
	error handling:
		- retuns without doing any changes if magic # of Deck was changed or if _deck is a NULL pointer.
*/
void DestroyDeck(struct Deck *_deck);

/*
	desc: Get cards stored in Deck struct.
	args: 
		-_deck: pointer to Deck struct.
	output: Address of pointer to cards member in Deck struct. 
	error handling:
*/
struct Card **GetCards(struct Deck *_deck);

# endif /* __CARDS_H__ */
