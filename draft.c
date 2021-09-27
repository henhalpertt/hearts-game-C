/*static void FillHand(struct Trick *_trick, struct Hand *_hand, int _from)*/
/*{*/
/*	int card;*/
/*	Rank rank;*/
/*	Suit suit;*/
/*	*/
/*	for(card= _from; card < _from+CARDS_EACH_HAND; card++)*/
/*	{*/
/*		rank = _trick->m_deck->m_cards[card]->m_rank;*/
/*		suit = _trick->m_deck->m_cards[card]->m_suit;*/
/*		_hand->m_cards[card - _from] = CreateCard(suit, rank);*/
/*	}*/
/*}*/

/*static struct Hand * CreateHand(int _playerID)*/
/*{*/
/*	struct Hand *newHand;*/
/*	struct Card **newCardsForPlayer;*/
/*	int from;*/
/*	*/
/*	newHand = (struct Hand*)malloc(sizeof(struct Hand));*/
/*	if(newHand == NULL)*/
/*	{*/
/*		return NULL;*/
/*	}*/
/*	newCardsForPlayer = (struct Card**)malloc(sizeof(struct Card) * CARDS_EACH_HAND);*/
/*	if(newCardsForPlayer == NULL)*/
/*	{*/
/*		return NULL;*/
/*	}*/
/*	from = CARDS_EACH_HAND * (_playerID);  */
/*	newHand->m_nCardsInHand = CARDS_EACH_HAND;*/
/*	newHand->m_cards = newCardsForPlayer;*/
/*	newHand->m_playerID = _playerID;*/
/*	return newHand;*/
/*}*/

/* 
static void SplitCardsToPlayers(struct Trick *_trick)
{	
	struct Hand **newHands;
	
	int player,card, nCards, nPlayers, from;
	if(_trick == 0)
	{
		return;
	}
	nCards = _trick->m_deck->m_nCards;
	nPlayers = _trick->m_team->m_totalPlayers;
	if(nCards != NUM_CARDS || nPlayers != VALID_NUM_PLAYERS)
	{
		printf("# of cards or players invalid");
		return;
	}

	newHands = (struct Hand**)malloc(sizeof(struct Hand) * 4);
	if(newHands == NULL)
	{
		return;
	}
	_trick->m_hands = newHands;
	/* from: [0-13), [13-26), [26-39), [39-52) */
	for(player=0, from=0; player<4; player++, from+=13)
	{
		_trick->m_hands[player] = CreateHand(player); /* new hand */ 
		FillHand(_trick, _trick->m_hands[player], from);
	}
	return;
}




static void SortEachHandByRank(struct Hand *_hand)
{
	size_t card, minIdx;
	int item;
	
	/* for every card in hand */
	for(card=0; card<CARDS_EACH_HAND-1; card++)
	{
		/* selection sort */
		minIdx = FindMinInSubsetByRank(_hand, card);
		SwapCards(_hand->m_cards[minIdx], _hand->m_cards[card]);
	}
}






static void ExchangeCardsCW(struct Trick *_trick)
{
	struct Card **bank;
	int player, cardCnt, bankCnt;
	
	bank = (struct Card**)malloc(sizeof(struct Card) * BANK_SIZE); /* 3 cards from each player */
	if(bank == NULL)
	{
		return;
	}
	
	_trick->m_bank = bank;
	bankCnt = 0;
	/*select 3 highest ranked cards from each player (last three cards in array) and store in bank */
	for(player=0; player<VALID_NUM_PLAYERS; player++)
	{
		for(cardCnt = 10; cardCnt<CARDS_EACH_HAND; cardCnt++)
		{
			_trick->m_bank[bankCnt] = _trick->m_hands[player]->m_cards[cardCnt];
			bankCnt++;
		}
	}
	/* Exchange 3 Cards among players */
	bankCnt = 0;
	for(player=1; player<VALID_NUM_PLAYERS; player++)
	{
			_trick->m_hands[player]->m_cards[10] = _trick->m_bank[bankCnt++];
			_trick->m_hands[player]->m_cards[11] = _trick->m_bank[bankCnt++];
			_trick->m_hands[player]->m_cards[12] = _trick->m_bank[bankCnt++];
	}
	/* handle the first player */
	_trick->m_hands[0]->m_cards[10] = _trick->m_bank[bankCnt++];
	_trick->m_hands[0]->m_cards[11] = _trick->m_bank[bankCnt++];
	_trick->m_hands[0]->m_cards[12] = _trick->m_bank[bankCnt++];

}

void FindImportantCards(struct Trick *_trick)
{
	int player, cardCnt;
	Rank rank;
	Suit suit;
	
	for(player=0; player<VALID_NUM_PLAYERS; player++)
	{
		for(cardCnt = 0; cardCnt<CARDS_EACH_HAND; cardCnt++)
		{
			rank = _trick->m_hands[player]->m_cards[cardCnt]->m_rank;
			suit = _trick->m_hands[player]->m_cards[cardCnt]->m_suit;
			if(rank == TWO && suit == CLUBS)
			{
				_trick->m_playerWClubsTwo = player;
			}
			if(rank == QUEEN && suit == SPADES)
			{
				_trick->m_playerWSpadesQueen = player;
			}
		}
	}
}

PassThreeCards(struct Trick *_trick)
{
	int player, cardCnt;
	for(player=0; player<VALID_NUM_PLAYERS; player++)
	{
		SortEachHandByRank(_trick->m_hands[player]);
	}	
	
	ExchangeCardsCW(_trick);
	
	/* re-sort according to game policy */
	for(player=0; player<VALID_NUM_PLAYERS; player++)
	{
		SortEachHand(_trick->m_hands[player]);
	}
	/* find player that holds 2 of cards & player w/ queen of spades */
	FindImportantCards(_trick);
}

static void RemoveCard(struct Trick *_trick, int _player, int _idx)
{
	int card;
	for(card = _idx; card<(_trick->m_hands[_player]->m_nCardsInHand)-1; card++)
	{
		_trick->m_hands[_player]->m_cards[card] = _trick->m_hands[_player]->m_cards[card+1];
	}
	_trick->m_hands[_player]->m_nCardsInHand--;
}

static void ChooseACard(struct Trick *_trick, int _player, int _leadingSuit)
{
	int rank, suit;
/*	rank = _trick->m_hands[_player]->m_cards[0]->m_rank;*/
/*	suit = _trick->m_hands[_player]->m_cards[0]->m_suit;*/
/*	printf("--> %d %d", rank, suit);*/
/*	if(_trick->m_heartsStatus != BROKEN)*/
/*	{*/
		/* find highest card in hand ? I will just take the 1st card for now */
	RemoveCard(_trick, _player, 12);
/*	}*/
	/* print after removal */
	int cardCnt;
		printf("AFTER REMOVAL: ID : %d \n", _player);
		for(cardCnt = 0; cardCnt<(_trick->m_hands[_player]->m_nCardsInHand); cardCnt++)
		{
			printf("suit: %d rank: %d \n\n", _trick->m_hands[_player]->m_cards[cardCnt]->m_suit,
										 _trick->m_hands[_player]->m_cards[cardCnt]->m_rank);
		}
		printf("\n");

}
static void StartTrick(struct Trick *_trick, int _leadingPlayer)
{
	int originalPlayer; /* originalPlayer - starting the trick */
	originalPlayer = _leadingPlayer;
	int leadingSuit;
	
	if(_trick->m_trickNumber == 1)
	{
		leadingSuit = CLUBS;
	}
	else
	{
		leadingSuit = SPADES;
	}
	/* cycle through players, starting with leading player
	 (player who won last round or 2 clubs p) */
	do
	{
		printf("Player: %d", _leadingPlayer%VALID_NUM_PLAYERS);
		/* cycle through players */
/*		ChooseACard(_trick, _leadingPlayer, leadingSuit);*/
		_leadingPlayer++;
		
	} while (_leadingPlayer % VALID_NUM_PLAYERS !=  originalPlayer);
	ChooseACard(_trick, originalPlayer, leadingSuit);
}

void RunGame(struct Trick *_trick)
{
	int player, trickWinnerID;
	player = _trick->m_playerWClubsTwo;
	/* trick # [1,13] */
	/*if its the 1st trick, then player with 2 clubs will start the game */
	StartTrick(_trick, player);
	_trick->m_trickNumber++;
	/*  trick [2-13] */
/*	while (_trick->m_trickNumber < 2)*/
/*	{*/
/*		StartTrick(_trick, trickWinnerID); */
/*		_trick->m_trickNumber++;*/
/*	}*/
}





/* fixed to round */

static void ExchangeCardsCW(struct Trick *_trick)
{
	struct Card **bank;
	int player, cardCnt, bankCnt;
	
	bank = (struct Card**)malloc(sizeof(struct Card) * BANK_SIZE); /* 3 cards from each player */
	if(bank == NULL)
	{
		return;
	}
	
	_trick->m_bank = bank;
	bankCnt = 0;
/*	select 3 highest ranked cards from each player (last three cards in array) and store in bank */
	for(player=0; player<VALID_NUM_PLAYERS; player++)
	{
		for(cardCnt = 10; cardCnt<CARDS_EACH_HAND; cardCnt++)
		{
			_trick->m_bank[bankCnt] = _trick->m_game->m_team->m_players[player]->m_cards[cardCnt];
			bankCnt++;
		}
	}
/*	 Exchange 3 Cards among players */
	bankCnt = 0;
	for(player=1; player<VALID_NUM_PLAYERS; player++)
	{
			_trick->m_game->m_team->m_players[player]->m_cards[10] = _trick->m_bank[bankCnt++];
			_trick->m_game->m_team->m_players[player]->m_cards[11] = _trick->m_bank[bankCnt++];
			_trick->m_game->m_team->m_players[player]->m_cards[12] = _trick->m_bank[bankCnt++];
	}
	/* handle the first player */
	_trick->m_game->m_team->m_players[0]->m_cards[10] = _trick->m_bank[bankCnt++];
	_trick->m_game->m_team->m_players[0]->m_cards[11] = _trick->m_bank[bankCnt++];
	_trick->m_game->m_team->m_players[0]->m_cards[12] = _trick->m_bank[bankCnt++];
}

void FindImportantCards(struct Trick *_trick)
{
	int player, cardCnt;
	Rank rank;
	Suit suit;
	
	for(player=0; player<VALID_NUM_PLAYERS; player++)
	{
		for(cardCnt = 0; cardCnt<CARDS_EACH_HAND; cardCnt++)
		{
			rank = _trick->m_game->m_team->m_players[player]->m_cards[cardCnt]->m_rank;
			suit = _trick->m_game->m_team->m_players[player]->m_cards[cardCnt]->m_suit;
			if(rank == TWO && suit == CLUBS)
			{
				_trick->m_playerWClubsTwo = player;
			}
			if(rank == QUEEN && suit == SPADES)
			{
				_trick->m_playerWSpadesQueen = player;
			}
		}
	}
}

PassThreeCards(struct Trick *_trick)
{
	int player, cardCnt;
	for(player=0; player<VALID_NUM_PLAYERS; player++)
	{
		SortEachHandByRank(_trick->m_game->m_team->m_players[player]);
	}	
	ExchangeCardsCW(_trick);
	
/*	 re-sort according to game policy */
	for(player=0; player<VALID_NUM_PLAYERS; player++)
	{
		SortEachHand(_trick->m_game->m_team->m_players[player]);
	}
/*	 find player that holds 2 of cards & player w/ queen of spades */
	FindImportantCards(_trick);
}


