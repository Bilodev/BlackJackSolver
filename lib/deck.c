#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "seed.h"

#define DECK_CARDS 52

struct deck {
    int* deck;
    int current_size;
    int decks_number;
};

void swap(int* a, int* b)
{
    int t = *a;
    *a = *b;
    *b = t;
}

void shuffleDeck(struct deck* d)
{
    for (size_t i = 0; i < d->current_size; i++) {
        int r1 = (i + rand()) % d->current_size;
        int r2 = (i + rand()) % d->current_size;
        swap(&d->deck[r1], &d->deck[r2]);
    }
}

struct deck* createDeck(int decks_number)
{
    srand(time(NULL));
    if (decks_number == 0) return NULL;
    struct deck* d = malloc(sizeof(struct deck));

    d->deck = malloc(sizeof(int) * DECK_CARDS * decks_number);

    d->current_size = 0;
    for (int j = 1; j <= DECK_CARDS * decks_number; j++) {
        d->deck[d->current_size] = j;
        d->current_size++;
    }

    d->decks_number = decks_number;
    shuffleDeck(d);
    return d;
}

int deckTop(struct deck* d) { return d->deck[--d->current_size]; }

void printDeckVal(struct deck* d)
{
    for (size_t i = 0; i < d->current_size; i++) printCard(d->deck[i]);
    printf("\n");
}

int getRemainingCards(struct deck* d) { return d->current_size; }