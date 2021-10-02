#ifndef __PLAYER_H__
#define __PLAYER_H__
#define VALID_NUM_PLAYERS 4

struct Team;
struct Player; 
struct Card;

/*
	desc: Create a Team struct. each team consists of n amount of players. 
		  Each player holds m amounf of cards, and its own player ID Number. 
	args: 
		-_nBots: # of bot players.
		-_nHumans:#of  human players.
		-_nCards: # of total cards for all player. 
					example: if _nCards = 52 and #of players = 4, then each player will have 13 cards 
		-__Cards: address to pointer of cards member. The member is in Team. 
	output: pointer to Team struct.
	error handling:
		- returns NULL if malloc for team pointer failed.
		- returns NULL if malloc for players pointer failed.
		- retunrs NULL if _nHumans and _nBots are both zero --> this check was moved to CreateGame()- see game.h
		
*/
struct Team * CreatePlayers(int _nBots, int _nHumans, int _nCards, struct Card **_cards);

/*
	desc: Sort Cards for each player's hand.
	args: 
		-_team: pointer to Team struct.
	output: 
	error handling:
		
*/
void SortCards(struct Team *_team);

/*
	desc: Sort Cards by rank for each player's hand.
	args: 
		-_team: pointer to Team struct.
	output: 
	error handling:
*/
void SortCardsByRank(struct Team *_team);
/*
	desc: Give a card from 1 player.
		  NOTE: This function also REMOVES that card from that player.
	args: 
		-_team: pointer to Team struct.
		-_playerID: player ID
		-_*card: address to store card's suit and rank.
		-PolicyGetCard - pointer to function.
		-_cardIdx - index of card
	output: 
	error handling:
		
*/
void PlayerGiveCard(struct Team *_team, int _playerId, int *card, int(*PolicyGetCard)(int), int _cardIdx);

/*
	desc: Retrieve the suit and rank of specific card at some index _idx, store it at address where *card points to.
		  NOTE: This function DOES NOT REMOVE that card from that player.
	args: 
		-_team: pointer to Team struct.
		-_playerID: player ID
		_*card: address to store card's suit and rank.
		-_idx: given index of desired card.
	output: 
	error handling:
		
*/
void PlayerSeeCard(struct Team *_team, int _playerId, int *card, int _idx);

/*
	desc: Give a card to a player. this will increase the number of cards in player. 
	args: 
		-_team: pointer to Team struct.
		-_playerID: player ID
		-_rank: rank of the card (TWO,....ACE)
		-_suit: suit of the card (HEARTS,SPADES, DIAMONDS, CLUBS)
	output: 
	error handling:
		
*/
void GiveCardsToPlayer(struct Team *_team, int _playerId, int rank, int suit);

/*
	desc: Find Player ID, store ID at address where *playerID points to. 
	args: 
		-_team: pointer to Team struct.
		-_rank: rank of the card (TWO,....ACE)
		-_suit: suit of the card (HEARTS,SPADES, DIAMONDS, CLUBS)
		*playerID: address to store player ID.
	output: 
	error handling:
		
*/
void FindPlayer(struct Team *_team, int _rank, int _suit, int *playerID);

/*
	desc: Find index of specific card given  a _rank and a _suit. stores index at address where *idx points to.   
	args: 
		-_team: pointer to Team struct.
		-_rank: rank of the card (TWO,....ACE)
		-_suit: suit of the card (HEARTS,SPADES, DIAMONDS, CLUBS)
		-*idx: pointer to index of card. 
	output: 
	error handling:
		
*/
void FindIdx(struct Team *_team, int _rank, int _suit, int *idx, int _player);

/*
	desc: Find the ideal card to give to round.c according to status of round and given rank of card.
	args: 
		-_team: pointer to Team struct.
		-_playerID: player ID
		-_leadRank: rank of the leading card (TWO,....ACE) -> not generic.
		-_leadSuit: suit of the leading card (HEARTS,SPADES, DIAMONDS, CLUBS) ->not generic.
		-*idx: pointer to index of card. 
		-_status: status of game (example: in game of hearts: status is BROKEN_NOT_BROKEN)
	output: 
	error handling:
		
*/
void FindBestCardIdx(struct Team *_team, int _playerID, int _leadSuit, int _leadRank, int *idx, int _status);

/*
	desc: Given a player ID, check if that player holds cards from some specific suit, _leadSuit. 
	args: 
		-_team: pointer to Team struct.
		-_playerID: player ID
		-_leadSuit: leading suit ->generic issue 
	output: SUIT_NOT_FOUND or SUIT_AVAILABLE
	error handling:
		
*/
int CheckSuitInHand(struct Team *_team, int _playerID, int _leadSuit);

/*
	desc: This function frees Players structs from memory and then frees team struct from memory.  
	args: 
		-_team: pointer to Team struct.
	output:
	error handling:
		
*/
void DestroyTeam(struct Team *_team);

/* print cards of the whole team (n players, where n = 1,2..,n) */
void PrintCards(struct Team *_team);
/* print cards of 1 player */ 
void PrintCardsHand(struct Team *_team, int _playerId);

#endif /* __PLAYER_H__ */
