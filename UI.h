#ifndef __UI_H__
#define __UI_H__
/*#include "SpecialChars.h"*/

/* print a string */
void PrintStr(char *str);

/* print a card with suit and rank */
void PrintCardUI(struct Card *_card);

/* print player ID */
void PrintIdUI(int _id);

/* print index of card in bold */
void PrintCardIdxUI(int _cardIdx);

/* print Scores of players */
void PrintScores(int *_scores, int _nPlayers);

/* Print card's' suit and rank in a specific color according to suit type */
void PrintTableCard(int _player, int _rank, int _suit);

/*user input - scanf */ 
void UserInput(int *_in);

# endif /* __UI_H__ */
