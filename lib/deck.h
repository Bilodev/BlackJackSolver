typedef struct deck* Deck;

Deck createDeck(int);
void shuffleDeck(Deck);
int deckTop(Deck);
void printDeckVal(Deck);
int getRemainingCards(Deck);