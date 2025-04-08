#include <raylib.h>
#include <stdio.h>

Texture2D cardbackTexture;
Texture2D chipsTextures[4];
Texture2D cardTextures[52 + 1];
int c = 1;
char s[20];

void loadTextures()
{
    cardbackTexture = LoadTexture("assets/defaultBack1.png");
    for (int j = 1; j <= 4; j++) {
        for (int i = 1; i <= 13; i++) {
            sprintf(s, "assets/%d-%d.png", i, j);
            cardTextures[c] = LoadTexture(s);
            c++;
        }
    }
    for (int i = 0; i < 4; i++) {
        sprintf(s, "assets/fishes%d.png", i);
        chipsTextures[i] = LoadTexture(s);
    }
}
