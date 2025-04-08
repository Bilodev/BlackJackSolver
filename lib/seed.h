enum seed { HEARTS, DIAMONDS, SPADES, CLUBS };

enum seed getCardSeed(int);

const char* getSeedName(enum seed s);

int getCardValue(int card);

void printCard(int);
