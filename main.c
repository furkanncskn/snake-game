#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h> 
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "snake_game.h"

#define MAX_SIZE     32

int main() {

    RANDOMIZED();

    openingMessage();

    char nick_name[MAX_SIZE];
    printf("Please, enter your nickname in the game = ");
    getsToBuffer(nick_name, MAX_SIZE);

    int rate_mode;
    printf("\nSLOW{0} - MEDIUM[1} - FAST{2}\n\nPlease, select rate = ");
    scanf("%d", &rate_mode);

    Rate rate;
    switch (rate_mode) {
    case 0:
        rate = SLOW;
        break;
    case 1:
        rate = MEDIUM;
        break;
    case 2:
        rate = FAST;
        break;
    default:
        fprintf(stderr, "Invalid rate mode entry\n");
        exit(EXIT_FAILURE);
        break;
    }

    system("cls");

    int _direct = STOP;
    HAREA area;
    HSNAKE snake;
    HBAIT bait;

    createObjects(&area, &snake, &bait);

    for (;;) {
        setRate(rate);
        setObjects(&area, &snake, &bait);
        keyPressedOrNot(&_direct);
        move(_direct, snake);
        eating(area, snake, bait, _direct);
        resetScreenPosition();
        printGameInfo(nick_name, getScore(), rate_mode);
        printArea(area);
    }

    deleteObjects(&area, &snake, &bait);

    return 0;
}