#define CARD_MAGIC_NUM 19931302
#define DECK_MAGIC_NUM 19931201
#define NUM_CARDS 52

typedef enum Suit
{
	HEARTS = 1,
	SPADES,
	DIAMONDS,
	CLUBS
} Suit;

typedef enum Rank
{
	TWO = 2,
	THREE,
	FOUR,
	FIVE,
	SIX,
	SEVEN,
	EIGHT,
	NINE,
	TEN,
	JACK,
	Queen,
	KING,
	ACE
} Rank;
