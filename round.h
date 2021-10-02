#ifndef __ROUND_H__
#define __ROUND_H__

/*
	desc: Setting up parameters before running a new trick. 
		  Parameters such as n# of bot players, # of real players, and creating a new Trick struct are performed here.
	args: 
		-_nPlayers - # of real players.
					 Example: if _nPlayers = 3, then there are 3 real players and 1 bot player. 
	output:
	error handling:
		- returns without running the round if _nPlayers exceeds VALID_NUM_PLAYERS for the game of hearts,
		  or if _nPlayers is negative. 
		  Note: _nPlayers can be zero. That only means that there are 4 bot players. 
		  
*/
void SetUpGame(int _nPlayers);

/*
	desc: will create a new Deck struct.
	args: 
		-_nCards: #of cards. 
	output: pointer to Deck Struct. 
	error handling:
*/
struct Deck * GetDeckForHearts(int _nCards);

/*
	desc: Get the valid number of cards to play this game. For hearts game, a valid number is 52 (FULL_DECK). 
	args: 
		-_game: pointer to struct Game.
	output: FULL_DECK 
	error handling:
*/
int GetAmntOfCardsHearts();

# endif /* __GAME_H__ */
