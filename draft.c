void RunARound(struct Trick *newTrick, int *_botOrHuman)
{
	int card[2], IdxOfTwoClubs, i;
	int originalPlayer, turn;
	int suits[TABLE_SIZE]={0}, ranks[TABLE_SIZE]={0}, bestIdx;
	int tmpRank = 0, curWinner;
	int roundScores[4] = {0}, heartCnt;
	int humanIdx, in=0;
	int result;
	PassThreeCards(newTrick->m_game);
	FindImportantCards(newTrick);
	turn = newTrick->m_playerWClubsTwo;
	i=0;
	do
	{
		printf("-------- trick # : %d\n", newTrick->m_trickNumber);
		originalPlayer = turn;
		if(newTrick->m_turnNum == 0)
		{
			if(newTrick->m_trickNumber == 0)
			{
				IdxOfTwoClubs = FindIdxOfCardViaRankSuit(newTrick->m_game, TWO, CLUBS, turn%VALID_NUM_PLAYERS);
				GetCard(newTrick->m_game, turn%VALID_NUM_PLAYERS, card, PolicyGetCard, IdxOfTwoClubs);
				newTrick->m_leadingSuit = CLUBS;
				newTrick->suits[i] = card[0];
				newTrick->ranks[i] = card[1];
			}
			else if(newTrick->m_trickNumber == 0)
			{	
				if(_botOrHuman[turn%VALID_NUM_PLAYERS] == BOT)
				{
					GetCard(newTrick->m_game, turn%VALID_NUM_PLAYERS, card, PolicyGetCard, 12-newTrick->m_trickNumber);
				}
				else
				{
					PrintHand(newTrick->m_game, turn%VALID_NUM_PLAYERS);
					while(in != 1)
					{
						/* print his hand */
						printf("1. Player #:%d put a card on the table(index starts from 0)", turn%VALID_NUM_PLAYERS);
						scanf("%d", &humanIdx);
						if(humanIdx < 0 || humanIdx > 13-newTrick->m_trickNumber)
						{
							printf("1. invalid card #");
						}
						else if(card[0] != CLUBS && card[1] != TWO)
						{
							printf("in the 1st trick, you must start with 2 of clubs ! ");
						}
						else
						{
							GetCard(newTrick->m_game, turn%VALID_NUM_PLAYERS, card, PolicyGetCard, humanIdx);
							in = 1;
						}
					}
				}
				newTrick->suits[i] = card[0];
				newTrick->ranks[i] = card[1];
				newTrick->m_leadingSuit = card[0];
			}
			if(newTrick->suits[i] == HEARTS && newTrick->m_heartsStatus != BROKEN)
			{
				printf("HEARTS STATUS BROKEN ! \n");
				newTrick->m_heartsStatus = BROKEN;
			}
			PrintTableCard(turn%VALID_NUM_PLAYERS, card[1], card[0]);
			i++;
			turn++;
			newTrick->m_turnNum++;
		}
		in = 0;
		/* 2,3,4 players */
		while (turn % VALID_NUM_PLAYERS !=  originalPlayer)
		{
			
			if(_botOrHuman[turn%VALID_NUM_PLAYERS] == BOT)
			{
				bestIdx = FindBestCard(newTrick, turn%VALID_NUM_PLAYERS, card[1]);
				GetCard(newTrick->m_game, turn%VALID_NUM_PLAYERS, card, PolicyGetCard, bestIdx);
			}
			else
			{
				PrintHand(newTrick->m_game, turn%VALID_NUM_PLAYERS);
				while(in != 1)
				{
					/* print his hand */
					printf("2. Player #:%d put a card on the table(index starts from 0)\n", turn%VALID_NUM_PLAYERS);
					scanf("%d", &humanIdx);
/*					c = getchar();*/
					if(humanIdx < 0 || humanIdx > 13-newTrick->m_trickNumber)
					{
						printf("2. invalid card #:humanIdx %d \n", humanIdx);
						continue;
					}
					SeeCardGame(newTrick->m_game, turn%VALID_NUM_PLAYERS, card, humanIdx);
					if(card[0] != newTrick->m_leadingSuit) /* card chosen is not from leading suit */
					{
						if(newTrick->m_heartsStatus != BROKEN)
						{
							CheckSuitGame(newTrick->m_game, turn%VALID_NUM_PLAYERS,newTrick->m_leadingSuit, &result);
							if(result == SUIT_NOT_FOUND)
							{
								newTrick->m_heartsStatus = BROKEN;
								GetCard(newTrick->m_game, turn%VALID_NUM_PLAYERS, card, PolicyGetCard, humanIdx);
								in = 1;
							}
							else
							{
								printf("Please put a card from the same suit ur card:%d \n", humanIdx);
							}
							continue;
						}
						else /*hearts status already been broken, put whatev you want */
						{
							GetCard(newTrick->m_game, turn%VALID_NUM_PLAYERS, card, PolicyGetCard, humanIdx);
							in = 1;
							continue;
						}
					}
					else
					{
						GetCard(newTrick->m_game, turn%VALID_NUM_PLAYERS, card, PolicyGetCard, humanIdx);
						in = 1;
					}
				}
			}
			newTrick->suits[i] = card[0];
			newTrick->ranks[i] = card[1];
			
			if(newTrick->suits[i] == HEARTS && newTrick->m_heartsStatus != BROKEN)
			{
				printf("HEARTS STATUS BROKEN ! \n");
				newTrick->m_heartsStatus = BROKEN;
			}
			PrintTableCard(turn%VALID_NUM_PLAYERS, card[1], card[0]);
			i++;
			turn++;
			newTrick->m_turnNum++;
		}
		
		/* player with highest ranked card "wins" the trick */		
		curWinner = FindWinner(newTrick->ranks, newTrick->suits, originalPlayer, &heartCnt);
		roundScores[curWinner] = roundScores[curWinner] + heartCnt;
		PrintScores(roundScores);
		newTrick->m_turnNum = 0;
		newTrick->m_trickNumber++;
		/* loser starts next round */
		turn=curWinner;
		heartCnt = 0;
		i=0;
		
	} while(newTrick->m_trickNumber < 13);
