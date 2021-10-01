#ifndef __UI_H__
#define __UI_H__
/*#include "SpecialChars.h"*/

void PrintStr(char *str);

void PrintCardUI(struct Card *_card);

void PrintIdUI(int _id);

void PrintCardIdxUI(int _cardIdx);

void PrintScores(int *_scores, int _nPlayers);

void PrintTableCard(int _player, int _rank, int _suit);

void PrintTable(int *suits, int *ranks);
# endif /* __UI_H__ */
