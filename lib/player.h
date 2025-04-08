#include <stdio.h>
#include <stdlib.h>

typedef struct player* Player;

struct player* createPlayer();

void addPlayerCard(Player, int card, int hand_number);
int isHandEmpty(Player);
void playerBet(Player, int amount);
int playerDouble(Player);
void playerHandSplit(Player);

int* getPlayerHand(Player, int* n, int hand_number);
int getPlayerChips(Player);
int getPlayerBet(Player);

void resetPlayerHand(Player);
void setPlayerChips(Player, int chips);
void addPlayerChips(Player, int chips);
void addToPlayerBet(struct player* p, int amount);
void printPlayerHand(Player);
int getHandValue(Player, int player_hand_number);
int isHandPair(Player, int hand);
int getHandSize(Player, int hand);
