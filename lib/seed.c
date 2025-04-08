#include "seed.h"

#include <stdio.h>

#define DECK_CARDS 52

enum seed getCardSeed(int card)
{
    card = card % DECK_CARDS;
    if (card == 0) card = 52;

    if (1 <= card && card <= 13)
        return HEARTS;
    else if (14 <= card && card <= 26)
        return DIAMONDS;
    else if (27 <= card && card <= 39)
        return SPADES;
    else if (40 <= card && card <= 52)
        return CLUBS;
}

int getCardValue(int card)
{
    int c = card % 13;
    if (c == 0) return 13;
    return c;
}

const char* getSeedName(enum seed s)
{
    switch (s) {
        case HEARTS:
            return "HEARTS";
        case DIAMONDS:
            return "DIAMONDS";
        case SPADES:
            return "SPADES";
        case CLUBS:
            return "CLUBS";
    }
}

void printCard(int card)
{
    int c = getCardValue(card);
    enum seed s = getCardSeed(card);
    printf("%d di %s\n", c, getSeedName(s));
}