#include <stdbool.h>

#include "player.h"
#include "seed.h"

enum moves { STAND, DOUBLE, SPLIT, HIT, CHOICE, NO_DEVIATIONS };

int high_low_counter = 0;
int true_counter = 0;
int card_counter = 0;

enum moves basic_strategy(Player house, Player player, int player_hand_number)
{
    int *user_hand, *house_hand;
    int size, sum = 0, house_card;

    house_hand = getPlayerHand(house, &size, 1);
    house_card = getCardValue(house_hand[1]);
    if (house_card == 1) house_card = 11;

    user_hand = getPlayerHand(player, &size, player_hand_number);

    bool soft_hand = false;
    int ace_index = -1;

    for (size_t i = 0; i < size; i++) {
        int card = getCardValue(user_hand[i]);
        if (card >= 10) card = 10;
        if (card == 1) {
            ace_index = i;
            soft_hand = true;
        }
        sum += card;
    }

    if (soft_hand && sum + 10 <= 21) sum += 10;
    if (!soft_hand && sum > 21) {
        return -1;
    }

    int first_card = getCardValue(user_hand[0]);
    int second_card = getCardValue(user_hand[1]);

    if (first_card == second_card && size == 2) {
        if (first_card == 1) return SPLIT;
        if (first_card == 10) return STAND;
        if (first_card == 9 && house_card != 7 && house_card != 10 &&
            house_card != 1)
            return SPLIT;
        if (first_card == 9) return STAND;
        if (first_card == 8) return SPLIT;
        if (first_card == 7 && 2 <= house_card && house_card <= 7) return SPLIT;
        if (first_card == 6 && 2 <= house_card && house_card <= 6) return SPLIT;
        if (first_card == 4 && (house_card == 5 || house_card == 6))
            return SPLIT;
        if ((first_card == 3 || first_card == 2) &&
            (2 <= house_card && house_card <= 7))
            return SPLIT;
    }

    if (soft_hand && size == 2) {
        int other_card_index = 1 - ace_index;
        int card = getCardValue(user_hand[other_card_index]);
        if (card == 9) return STAND;
        if (card == 8 && house_card == 6) return DOUBLE;
        if (card == 8) return STAND;
        if (card == 7 && 2 <= house_card && house_card <= 6) return DOUBLE;
        if (card == 7 && 7 <= house_card && house_card <= 8) return STAND;
        if (card == 7) return HIT;
        if (card == 6 && 3 <= house_card && house_card <= 6) return DOUBLE;
        if (card == 6) return HIT;
        if (card == 5 && 4 <= house_card && house_card <= 6) return DOUBLE;
        if (card == 5) return HIT;
        if (card == 4 && 4 <= house_card && house_card <= 6) return DOUBLE;
        if (card == 4) return HIT;
        if (card == 3 && 5 <= house_card && house_card <= 6) return DOUBLE;
        if (card == 3) return HIT;
        if (card == 2 && 5 <= house_card && house_card <= 6) return DOUBLE;
        if (card == 2) return HIT;
    }
    // if (!soft_hand) {
    if (sum <= 7) return HIT;
    if (sum >= 17) return STAND;
    if ((13 <= sum && sum <= 16) && 2 <= house_card && house_card <= 6)
        return STAND;

    if ((13 <= sum && sum <= 16) && (7 <= house_card && house_card <= 11))
        return HIT;
    if (sum == 12 && 4 <= house_card && house_card <= 6) return STAND;
    if (sum == 12) return HIT;
    if (sum == 11) return DOUBLE;
    if (sum == 10 && (house_card == 1 || house_card == 10)) return HIT;
    if (sum == 10) return DOUBLE;
    if (sum == 9 && 3 <= house_card && house_card <= 6) return DOUBLE;
    if (sum == 8) return HIT;
    if (sum <= house_card) return HIT;
    // }
    return CHOICE;
}
int get_true_count() { return true_counter; }

void count_card(int card, const int decks_number)
{
    if (decks_number == 0) return;
    card_counter++;
    card = getCardValue(card);
    if (card >= 10) card = 10;
    if (2 <= card && card <= 6) high_low_counter++;
    if (10 == card || card == 1) high_low_counter--;
    int remaining_decks = decks_number - (card_counter / 52);
    true_counter = high_low_counter / remaining_decks;
}

char* getMoveName(enum moves m)
{
    switch (m) {
        case STAND:
            return "STAND";
        case HIT:
            return "HIT";
        case SPLIT:
            return "SPLIT";
        case DOUBLE:
            return "DOUBLE";
        case CHOICE:
            return "YOUR CHOICE";
        case NO_DEVIATIONS:
            return "No Deviations";
    }
}

enum moves deviations(Player house, Player player, int player_hand_number)
{
    int size;

    int* house_hand = getPlayerHand(house, &size, 1);
    int house_card = getCardValue(house_hand[1]);
    if (house_card == 1) house_card = 11;

    bool soft_hand = false;
    int ace_index = -1;

    int sum = getHandValue(player, player_hand_number);

    int* user_hand = getPlayerHand(player, &size, player_hand_number);
    int first_card = getCardValue(user_hand[0]);
    int second_card = getCardValue(user_hand[1]);

    if (first_card == 1) {
        soft_hand = true;
        ace_index = 0;
    }
    else if (second_card == 1) {
        soft_hand == true;
        ace_index = 1;
    }

    if (isHandPair(player, player_hand_number) &&
        getHandValue(player, player_hand_number) == 20) {
        if (true_counter >= 6 && house_card == 4) return SPLIT;
        if (true_counter >= 5 && house_card == 5) return SPLIT;
        if (true_counter >= 4 && house_card == 6) return SPLIT;
    }
    if (soft_hand && size == 2) {
        int other_card_index = 1 - ace_index;
        int card = getCardValue(user_hand[other_card_index]);
        if (card == 8 && true_counter >= 3 && house_card == 4) return DOUBLE;
        if (card == 8 && true_counter >= 1 && house_card == 5) return DOUBLE;
        if (card == 8 && true_counter <= 0 && house_card == 6) return DOUBLE;
        if (card == 6 && true_counter >= 1 && house_card == 2) return DOUBLE;
    }
    if (sum == 16 && true_counter >= 4 && house_card == 9) return HIT;
    if (sum == 16 && true_counter >= 0 && house_card == 10) return HIT;
    if (sum == 16 && true_counter >= 3 && house_card == 1) return HIT;

    if (sum == 15 && true_counter >= 5 && house_card == 1) return HIT;
    if (sum == 15 && true_counter >= 4 && house_card == 10) return HIT;

    if (sum == 13 && true_counter <= -1 && house_card == 2) return HIT;

    if (sum == 12 && true_counter >= 3 && house_card == 2) return HIT;
    if (sum == 12 && true_counter >= 2 && house_card == 3) return HIT;

    if (sum == 10 && true_counter >= 3 && house_card == 1) return HIT;
    if (sum == 10 && true_counter >= 4 && house_card == 10) return HIT;

    if (sum == 9 && true_counter >= 1 && house_card == 2) return HIT;
    if (sum == 9 && true_counter >= 3 && house_card == 7) return HIT;

    if (sum == 8 && true_counter >= 2 && house_card == 6) return HIT;

    return NO_DEVIATIONS;
}