#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "snake_game.h"

#define RANDOMIZED() (srand((unsigned)time(NULL)))  // for random game

#define MAX_SIZE     32

int main()
{
    RANDOMIZED();
    
    opening_message();
    
    char nick_name[MAX_SIZE];

    printf("Please, enter your nickname in the game = ");

    if(gets_s(nick_name, MAX_SIZE))
        play_snake_game(nick_name);

	return 0;
}