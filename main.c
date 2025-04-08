#include <raylib.h>
#include <stdio.h>
#include <string.h>

#include "lib/AI.h"
#include "lib/deck.h"
#include "lib/player.h"
#include "lib/queue.h"
#include "lib/seed.h"
#include "lib/textures.h"

#define WIDTH 1920
#define HEIGHT 1080
#define DECKS_NUMBER 2

#define KICKED_STR "You got kicked out of the Casino!!!!"
#define ESC_STR "Press ESC to Leave"

int ended_turn = 1;
int can_user_play = 0;
int turns_played = 0;
int covered_card_counted = 0;
char chips_asset_string[20];

char chips_str[5];
int kicked = 0;

Rectangle start_rect = {1920 - 325, HEIGHT - 130, 280, 80};

Rectangle hit_rect = {700, HEIGHT / 2, 130, 60};
Rectangle double_rect = {900, HEIGHT / 2, 130, 60};
Rectangle stand_rect = {1100, HEIGHT / 2, 130, 60};
Rectangle split_rect = {1100, HEIGHT - 300, 130, 60};

Rectangle minus_rect = {WIDTH / 2 + 145, HEIGHT - 60, 30, 30};
Rectangle plus_rect = {WIDTH / 2 + 215, HEIGHT - 60, 30, 30};

Queue animationsCard;

Player dealer, player;

int current_player_hand;
int splitted;

char win_strs[2][30];

int waited = 0;

struct animation* createAnimation(int card, int sx, int sy, int ex, int ey,
                                  Player p, int hand)
{
    struct animation* a = malloc(sizeof(struct animation));
    a->card = card;
    a->dest = p;
    a->dx = a->ddx = abs(sx - ex);
    a->dy = a->ddy = abs(sy - ey);
    a->dest_hand = hand;
    return a;
}
void drawCard(Texture2D card_texture, int x, int y);

void animateCards()
{
    struct animation* a = front(animationsCard);
    if (a == NULL) return;
    int card_id = getCardValue(a->card) + (getCardSeed(a->card) * 13);

    Texture2D card_texture = cardbackTexture;

    int x = 0, y = 0;
    if (--a->ddx <= 0)
        x = a->dx;
    else
        x = a->dx - a->ddx + 10;
    if (--a->ddy <= 0)
        y = a->dy;
    else
        y = a->dy - a->ddy - 4;
    drawCard(card_texture, 1300 - x, 200 + y);
    if (a->ddx <= 0 && 0 >= a->ddy) {
        dequeue(animationsCard);
        addPlayerCard(a->dest, a->card, a->dest_hand);
        if (!(can_user_play && getPlayerHand(dealer, &x, 1)[0] == a->card))
            count_card(a->card, DECKS_NUMBER);
    }
}

void initTurn(Player players[], Deck deck)
{
    if (getRemainingCards(deck) < 10) {
        deck = createDeck(DECKS_NUMBER);
    }
    covered_card_counted = 0;
    resetPlayerHand(players[0]);
    resetPlayerHand(players[1]);

    // da una carta a testa
    for (size_t i = 0; i < 2 * 2; i++) {
        int c = deckTop(deck);
        int ex = WIDTH / 2,
            ey = 800 + 20 * getHandSize(players[i % 2], current_player_hand);

        // per posizionare carta dealer
        if ((i % 2) == 0) ey -= 600;

        c = deckTop(deck);
        enqueue(animationsCard,
                createAnimation(c, 1300, 200, ex, ey, players[i % 2], 1));
    }
}

int fishesToImage(int fishes, int* times)
{
    if (fishes == 0) return -1;
    *times = fishes / 1000;
    fishes %= 1000;
    if (fishes == 0) return -1;
    if (0 <= fishes && fishes <= 200) return 0;
    if (200 <= fishes && fishes <= 500) return 1;
    if (500 <= fishes && fishes <= 1000) return 2;
}

void UILoop(Player players[])
{
    for (size_t i = 0; i < 2; i++) {
        int hand_size1, hand_size2 = 0;

        int* player_hand1 = getPlayerHand(players[i], &hand_size1, 1);

        int* player_hand2 =
            i == 0 ? NULL : getPlayerHand(players[i], &hand_size2, 2);

        int y = (i == 0) ? 200 : 800;
        // set up delle due mani se splittato
        int x = (splitted && i != 0) ? 100 : 0;
        for (size_t j = 0; j < hand_size1; j++) {
            int card = player_hand1[j];

            // prima carta del dealer
            Texture2D card_texture;
            if (i == 0 && j == 0 && can_user_play) {
                card_texture = cardbackTexture;
            }
            else if (!can_user_play && i == 0 && j == 0 &&
                     !covered_card_counted) {
                count_card(card, DECKS_NUMBER);
                covered_card_counted = 1;
            }
            else {
                card = getCardValue(card) + (getCardSeed(card) * 13);
                card_texture = cardTextures[card];
            }

            drawCard(card_texture, WIDTH / 2 - x * 2, y + (20 * j));
        }

        for (size_t j = 0; player_hand2 && j < hand_size2; j++) {
            int card = player_hand2[j];

            card = getCardValue(card) + (getCardSeed(card) * 13);
            Texture2D card_texture = cardTextures[card];

            drawCard(card_texture, WIDTH / 2 + x * 2, y + (20 * j));
        }

        // if (!isEmptyQueue(animations)) continue;

        // nascondi punteggio carta del dealer

        if (i == 0 && getHandSize(dealer, 1) <= 1) continue;

        int t = getHandValue(players[i], 1);

        if (i == 0 && can_user_play) {
            int c = getCardValue(player_hand1[0]);
            if (c >= 10) c = 10;

            if (c == 1)
                t -= 11;
            else
                t -= c;
            c = getCardValue(player_hand1[1]);
            if (c == 1) {
                t = 11;
            }
        }

        char hand_value[3];
        Color c = (t == 21) ? YELLOW : WHITE;
        if (splitted && current_player_hand == 1 && !ended_turn && i != 0)
            c = BLUE;
        if (!ended_turn && !can_user_play && splitted &&
            current_player_hand == 1 && i != 0)
            c = BLUE;
        if (t > 21) c = RED;

        sprintf(hand_value, "%d", t);

        if (t > 0) DrawText(hand_value, WIDTH / 2 - 100, y + 100, 20, c);

        if (i != 0 && splitted) {
            t = getHandValue(players[i], 2);

            if (i == 0 && can_user_play) {
                int c = getCardValue(player_hand1[0]);
                if (c >= 10) c = 10;

                if (c == 1)
                    t -= 11;
                else
                    t -= c;
            }

            char hand_value[3];
            Color c = (t == 21) ? YELLOW : WHITE;
            if (splitted && current_player_hand == 2 && !ended_turn && i != 0)
                c = BLUE;
            if (!ended_turn && !can_user_play && splitted &&
                current_player_hand == 2 && i != 0)
                c = BLUE;
            if (t > 21) c = RED;

            sprintf(hand_value, "%d", t);
            if (t > 0) DrawText(hand_value, WIDTH / 2 + 100, y + 100, 20, c);
        }
    }
    if (getHandValue(players[1], current_player_hand) >= 21) {
        if (splitted && current_player_hand == 1)
            current_player_hand = 2;
        else
            can_user_play = 0;
    };

    if (!can_user_play || !isEmptyQueue(animationsCard)) return;

    // Disegno bottoni

    int first_card = getCardValue(getPlayerHand(player, &c, 1)[0]);

    if (!splitted || (splitted && first_card != 1)) {
        DrawRectangle(hit_rect.x, hit_rect.y, hit_rect.width, hit_rect.height,
                      WHITE);

        DrawText("HIT", hit_rect.x + 35, hit_rect.y + 12, 30, BLACK);
    }

    if (getHandSize(players[1], 1) < 3 &&
        getPlayerChips(players[1]) >= getPlayerBet(players[1]) && !splitted) {
        DrawRectangle(double_rect.x, double_rect.y, double_rect.width,
                      double_rect.height, WHITE);
        DrawText("DOUBLE", double_rect.x + 5, double_rect.y + 12, 30, BLACK);
    }

    DrawRectangle(stand_rect.x, stand_rect.y, stand_rect.width,
                  stand_rect.height, WHITE);
    DrawText("STAND", stand_rect.x + 8, stand_rect.y + 12, 30, BLACK);

    if (isHandPair(players[1], 1) &&
        getPlayerChips(players[1]) >= getPlayerBet(players[1]) && !splitted) {
        DrawRectangle(split_rect.x, split_rect.y, split_rect.width,
                      split_rect.height, WHITE);
        DrawText("SPLIT", split_rect.x + 15, split_rect.y + 12, 30, BLACK);
    }

    int move_int = basic_strategy(players[0], players[1], current_player_hand);
    char* move_str = getMoveName(move_int);
    DrawText(move_str, 200, HEIGHT / 2, 40, WHITE);

    char count_str[10];
    int c = get_true_count();
    sprintf(count_str, "%d", c);
    DrawText(count_str, 200, HEIGHT / 2 + 40, 40, WHITE);

    move_int = deviations(dealer, player, current_player_hand);
    move_str = getMoveName(move_int);
    DrawText(move_str, 200, HEIGHT / 2 + 80, 40, WHITE);
}

void userEvents(Player player, Deck deck)
{
    if (!can_user_play) return;
    int i;
    int first_card = getCardValue(getPlayerHand(player, &i, 1)[0]);
    if (CheckCollisionPointRec(GetMousePosition(), hit_rect) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (splitted && first_card == 1) return;
        int c = deckTop(deck);
        int x = (splitted) ? 100 : 0;
        x = (current_player_hand == 1) ? x * -1 : x;
        enqueue(
            animationsCard,
            createAnimation(c, 1300, 200, WIDTH / 2 + 2 * x,
                            800 + 20 * getHandSize(player, current_player_hand),
                            player, current_player_hand));
    }
    if (CheckCollisionPointRec(GetMousePosition(), double_rect) &&
        getPlayerChips(player) >= getPlayerBet(player) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        int c = deckTop(deck);

        enqueue(
            animationsCard,
            createAnimation(c, 1300, 200, WIDTH / 2,
                            800 + 20 * getHandSize(player, current_player_hand),
                            player, current_player_hand));

        addToPlayerBet(player, getPlayerBet(player));
    }
    if (CheckCollisionPointRec(GetMousePosition(), stand_rect) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (current_player_hand == 2 || !splitted)
            can_user_play = 0;
        else if (splitted)
            current_player_hand = 2;
    }
    if (CheckCollisionPointRec(GetMousePosition(), split_rect) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && isHandPair(player, 1)) {
        splitted = 1;
        addToPlayerBet(player, getPlayerBet(player));
        playerHandSplit(player);
        int c = deckTop(deck);

        int x = (splitted) ? 100 : 0;
        enqueue(animationsCard,
                createAnimation(c, 1300, 200, WIDTH / 2 - x * 2,
                                800 + 20 * getHandSize(player, 1), player, 1));

        c = deckTop(deck);
        enqueue(animationsCard,
                createAnimation(c, 1300, 200, WIDTH / 2 + x * 2,
                                800 + 20 * getHandSize(player, 2), player, 2));
    }
}

void payPlayers(Player players[], int hand)
{
    Player dealer = players[0];
    Player player = players[1];

    int dealerHand = getHandValue(dealer, 1);
    int player_bet = getPlayerBet(player);
    if (splitted) player_bet /= 2;

    int win = 0, tie = 0;

    if (hand == 2 && !splitted) return;
    int userHand = getHandValue(player, hand);
    int cards_number = getHandSize(player, hand);

    if (dealerHand > 21) win = 1;
    if (userHand > dealerHand && userHand <= 21) win = 1;
    if (userHand == dealerHand && userHand <= 21 && dealerHand <= 21) tie = 1;

    if (tie) {
        addPlayerChips(player, player_bet);
        strcpy(win_strs[hand - 1], "TIE");
    }

    else if (win) {
        if (userHand == 21 && cards_number == 2 && !splitted)
            addPlayerChips(player, player_bet + (player_bet * 3) / 2);
        else
            addPlayerChips(player, player_bet * 2);
        strcpy(win_strs[hand - 1], "WON");
    }
    else if (!win && !tie) {
        strcpy(win_strs[hand - 1], "LOST");
    }
    win = tie = 0;

    if (hand == 1)
        if (getPlayerChips(player) >= 100)
            playerBet(players[1], 100);
        else
            playerBet(players[1], 0);
}

void house_play(Player players[], Deck deck, int p_hand)
{
    if (!isEmptyQueue(animationsCard)) return;
    if (ended_turn) return;
    int dealer_hand = getHandValue(players[0], 1);

    int player_hand = getHandValue(players[1], p_hand);

    if (player_hand >= 21) {
        if (splitted && current_player_hand == 2)
            current_player_hand = 1;
        else {
            ended_turn = 1;
        }
        payPlayers(players, p_hand);
        return;
    }

    if ((dealer_hand < player_hand && player_hand <= 21) ||
        (dealer_hand == player_hand && dealer_hand <= 13)) {
        int added_card = deckTop(deck);

        enqueue(animationsCard,
                createAnimation(added_card, 1300, 200, WIDTH / 2,
                                200 + 20 * getHandSize(players[0], 1),
                                players[0], 1));
    }
    if (dealer_hand >= player_hand) {
        if (splitted && current_player_hand == 2)
            current_player_hand = 1;
        else {
            ended_turn = 1;
        }
        payPlayers(players, p_hand);
    }
}

void format_int(char* s, int val) { sprintf(s, "%.4d", val); }

void drawChips(Player player, int amount, int x)
{
    int times = 0;
    int textureId = fishesToImage(amount, &times);
    for (size_t i = 0; i < times; i++) {
        Texture2D t = chipsTextures[3];
        DrawTexture(t, WIDTH / 2 + x + (i * 40), HEIGHT - 160, WHITE);
    }
    if (textureId != -1) {
        Texture2D t = chipsTextures[textureId];
        int offset = (textureId == 0) ? 10 : 0;
        DrawTexture(t, WIDTH / 2 + x + offset + (times * 40), HEIGHT - 160,
                    WHITE);
    }
    format_int(chips_str, amount);
    DrawText(chips_str, WIDTH / 2 + x, HEIGHT - 100, 40, WHITE);
}

int main()
{
    SetTraceLogLevel(100);
    InitWindow(WIDTH, HEIGHT, "BlackJack");
    SetTargetFPS(600);
    loadTextures();
    dealer = createPlayer();
    player = createPlayer();
    Player players[] = {dealer, player};
    Deck house_deck = createDeck(DECKS_NUMBER);

    Texture2D deck_texture = LoadTexture("assets/reddeck.png");

    animationsCard = newQueue();

    ToggleFullscreen();
    while (!WindowShouldClose()) {
        BeginDrawing();
        if (!can_user_play && !ended_turn && current_player_hand == 1 &&
            splitted && !waited && isEmptyQueue(animationsCard)) {
            waited = 1;
            WaitTime(1.5);
        }

        animateCards();

        if (!kicked) {
            if (ended_turn &&
                CheckCollisionPointRec(GetMousePosition(), minus_rect) &&
                IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                int t = getPlayerBet(player);
                if (t >= 100) addToPlayerBet(player, -100);
            }
            if (ended_turn &&
                CheckCollisionPointRec(GetMousePosition(), plus_rect) &&
                IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                int t = getPlayerBet(player);
                if (getPlayerChips(player) >= 100) addToPlayerBet(player, 100);
            }
            if (CheckCollisionPointRec(GetMousePosition(), start_rect) &&
                IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && ended_turn &&
                getPlayerBet(player) > 0) {
                ended_turn = splitted = 0;
                can_user_play = current_player_hand = 1;
                turns_played++;
                initTurn(players, house_deck);
                waited = 0;
                strcpy(win_strs[0], "");
                strcpy(win_strs[1], "");
            }
        }

        DrawText("BlackJack", 10, 10, 40, WHITE);

        DrawText("DEALER", WIDTH / 2 - 20 * 6, 10, 40, WHITE);
        DrawText("YOU", WIDTH / 2 - 20 * 3, HEIGHT - 100, 40, WHITE);
        drawCard(deck_texture, 1300, 200);

        if (turns_played != 0) UILoop(players);
        if (!kicked) userEvents(player, house_deck);

        if (!kicked) {
            if (!can_user_play && splitted && current_player_hand == 2)
                house_play(players, house_deck, 2);

            if (!can_user_play && current_player_hand == 1)
                house_play(players, house_deck, 1);
        }

        if (ended_turn && !kicked) {
            DrawRectangle(start_rect.x, start_rect.y, start_rect.width,
                          start_rect.height, BLACK);
            DrawText("Start New Turn", 1920 - 305, HEIGHT - 105, 30, WHITE);

            DrawRectangle(plus_rect.x, plus_rect.y, plus_rect.width,
                          plus_rect.height, WHITE);
            DrawText("+", plus_rect.x + 5, plus_rect.y - 2, 40, BLACK);

            DrawRectangle(minus_rect.x, minus_rect.y, minus_rect.width,
                          minus_rect.height, WHITE);
            DrawText("-", minus_rect.x + 7, minus_rect.y - 2, 40, BLACK);
        }

        drawChips(player, getPlayerChips(player), -350);
        drawChips(player, getPlayerBet(player), 150);

        if (getPlayerChips(player) + getPlayerBet(player) >= 9000 ||
            (getPlayerChips(player) + getPlayerBet(player) < 100)) {
            kicked = 1;
            DrawText(KICKED_STR, WIDTH / 2 - MeasureText(KICKED_STR, 50) / 2,
                     HEIGHT / 2, 50, WHITE);
            DrawText(ESC_STR, WIDTH / 2 - MeasureText(ESC_STR, 50) / 2,
                     HEIGHT / 2 + 50, 50, WHITE);
        }

        int x = (splitted) ? 100 : 0;
        if (win_strs[0]) {
            DrawText(win_strs[0],
                     WIDTH / 2 - MeasureText(win_strs[0], 30) / 2 - 2 * x,
                     HEIGHT / 2 + 130, 30, WHITE);
        }
        if (win_strs[1])
            DrawText(win_strs[1],
                     WIDTH / 2 - MeasureText(win_strs[1], 30) / 2 + x * 2,
                     HEIGHT / 2 + 130, 30, WHITE);

        ClearBackground(DARKGREEN);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}

void drawCard(Texture2D card_texture, int x, int y)
{
    int frameWidth = card_texture.width;
    int frameHeight = card_texture.height;
    Rectangle sourceRec = {0.0f, 0.0f, (float)frameWidth, (float)frameHeight};
    Vector2 origin = {(float)frameWidth, (float)frameHeight};
    Rectangle destRec = {x, y, frameWidth * 2.0f, frameHeight * 2.0f};
    DrawTexturePro(card_texture, sourceRec, destRec, origin, 0, WHITE);
}
