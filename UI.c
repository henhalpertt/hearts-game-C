#include <stdio.h>
#include "card.h"
#include "UI.h"
void PrintStrUI(char *str)
{
	printf("%s", str);
}

void PrintValueUI(char *str, int _value)
{
	printf("%s%d\n", str, _value);
}

void PrintPlayerAndScore(int _id, int _score)
{
	printf("Player #%d, Score:%d\n", _id, _score);
}

void PrintCardUI(struct Card *_card)
{
	if(GetSuit(_card) == HEARTS)
	{
		printf("%s%s%d%s ", BOLD_ON, RED, GetRank(_card), HEART);
		printf(" ");
		printf("%s", WHITE);
		return;
	}
	if(GetSuit(_card) == SPADES)
	{
		printf("%s%s%d%s ", BOLD_ON, YELLOW, GetRank(_card), SPADE);
		printf(" ");
		printf("%s", WHITE);
		return;
	}
	if(GetSuit(_card) == CLUBS)
	{
		printf("%s%s%d%s ", BOLD_ON, BLUE, GetRank(_card), CLUB);
		printf(" ");
		printf("%s", WHITE);
		return;
	}
	if(GetSuit(_card) == DIAMONDS)
	{
		printf("%s%s%d%s ", BOLD_ON, CYAN, GetRank(_card), DIAMOND);
		printf(" ");
		printf("%s", WHITE);
		return;
	}
}

void SuitNumToSuitNameUI(int _suit)
{
	switch (_suit)
	{
		case (HEARTS):
			printf("Leading Suit: hearts");
			break;
		case (SPADES):
			printf("Leading Suit: spades");
			break;
		case (DIAMONDS):
			printf("Leading Suit: Diamonds");
			break;
		case (CLUBS):
			printf("Leading Suit: Clubs");
			break;
	}
}

void PrintIdUI(int _id)
{
	printf("Player #%d\n", _id+1);
}

void PrintCardIdxUI(int _cardIdx)
{
	printf("%s%d.",BOLD_ON, _cardIdx);
}

void PrintScores(int *_scores, int _nPlayers)
{
	int nPlayers, score;
	for(score=0; score<_nPlayers; score++)
	{
		printf("player: %d Score: %d \n", score+1, _scores[score]);
	}
}

void PrintTableCard(int _player, int _rank, int _suit)
{
	switch (_suit)
	{
		case (HEARTS):
			printf("%s%splayer #%d selected %s%d%s\n",BOLD_ON, GREEN,_player+1, RED, _rank, HEART);
			printf(" ");
			printf("%s", WHITE);
			return;
		case (SPADES):
			printf("%splayer #%d selected %s%d%s\n",BOLD_ON,_player+1, YELLOW,  _rank, SPADE);
			printf(" ");
			printf("%s", WHITE);
			return;
		case (CLUBS):
			printf("%s%splayer #%d selected %s%d%s\n",BOLD_ON,GREEN,_player+1, BLUE, _rank, CLUB);
			printf(" ");
			printf("%s", WHITE);
			return;
		case (DIAMONDS):
			printf("%splayer #%d selected %s%d%s\n",BOLD_ON,_player+1, CYAN, _rank, DIAMOND);
			printf(" ");
			printf("%s", WHITE);
			return;
	}
}

void UserInput(int *_in)
{
	scanf("%d", _in);
}


