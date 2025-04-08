#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "seed.h"

struct player {
    int hand1[10];
    int hand1_size;

    int hand2[10];
    int hand2_size;

    int chips;
    int current_bet;
};

struct player* createPlayer()
{
    struct player* p = malloc(sizeof(struct player));
    p->hand1_size = 0;
    p->hand2_size = 0;
    p->chips = 900;
    p->current_bet = 100;
    return p;
}
int* getPlayerHand(struct player* p, int* n, int hand_number)
{
    if (hand_number == 1) {
        *n = p->hand1_size;
        return p->hand1;
    }
    else if (hand_number == 2) {
        *n = p->hand2_size;
        return p->hand2;
    }
    return NULL;
};

int isHandEmpty(struct player* p) { return p->hand1_size == 0; }

void resetPlayerHand(struct player* p)
{
    p->hand1_size = 0;
    p->hand2_size = 0;
}

int getPlayerChips(struct player* p) { return p->chips; }

void setPlayerChips(struct player* p, int chips) { p->chips = chips; }

void addPlayerCard(struct player* p, int card, int hand_number)
{
    if (hand_number == 1) {
        p->hand1[p->hand1_size++] = card;
    }
    else if (hand_number == 2) {
        p->hand2[p->hand2_size++] = card;
    }
}

void printPlayerHand(struct player* p)
{
    printf("----------\n1.\n");
    for (size_t i = 0; i < p->hand1_size; i++) printCard(p->hand1[i]);
    printf("\n");

    printf("2.\n");
    for (size_t i = 0; i < p->hand2_size; i++) printCard(p->hand2[i]);
    printf("\n");
}

void playerBet(struct player* p, int amount)
{
    if (amount <= p->chips) {
        p->current_bet = amount;
        p->chips -= amount;
    };
}

void addPlayerChips(struct player* p, int chips) { p->chips += chips; }
int getPlayerBet(struct player* p) { return p->current_bet; }

int playerDouble(struct player* p)
{
    p->current_bet *= 2;
    return getPlayerBet(p);
}

void playerHandSplit(struct player* p)
{
    p->hand2[0] = p->hand1[1];
    p->hand1_size = p->hand2_size = 1;
}

int getHandValue(struct player* p, int player_hand_number)
{
    int size, sum = 0;
    int* user_hand = getPlayerHand(p, &size, player_hand_number);
    if (user_hand == NULL) return 0;
    bool soft_hand = false;

    for (size_t i = 0; i < size; i++) {
        int card = getCardValue(user_hand[i]);
        if (card >= 10) card = 10;
        if (card == 1) {
            soft_hand = true;
        }
        sum += card;
    }

    if (soft_hand && sum + 10 <= 21) sum += 10;
    return sum;
}
int getHandSize(struct player* p, int hand)
{
    if (hand == 1) return p->hand1_size;
    if (hand == 2) return p->hand2_size;
    return 0;
}

int isHandPair(struct player* p, int player_hand_number)
{
    if (getHandSize(p, player_hand_number) > 2) return 0;
    if (player_hand_number == 1 && p->hand1_size == 2)
        return getCardValue(p->hand1[0]) == getCardValue(p->hand1[1]);
    else if (player_hand_number == 2 && p->hand2_size == 2)
        return getCardValue(p->hand2[0]) == getCardValue(p->hand2[1]);
    return 0;
}

void addToPlayerBet(struct player* p, int amount)
{
    p->current_bet += amount;
    p->chips -= amount;
}