#include "player.h"

enum moves { STAND, DOUBLE, SPLIT, HIT, CHOICE, NO_DEVIATIONS };

enum moves basic_strategy(Player, Player, int hand_number);

char* getMoveName(enum moves);
void count_card(int, int);
int get_true_count();
enum moves deviations(Player, Player, int);
