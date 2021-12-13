#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h> // Sleep()
#include <stdlib.h>  // srand(), rand()
#include <stdio.h>
#include <conio.h>   // kbhit(), _getch()
#include <ctype.h>   // toupper()
#include <time.h>    // time()

#include "snake_game.h"

#define PUBLIC 
#define PRIVATE static

/*!< Functional Like Macro Definitions */
#define ISVALIDKEY(temp_direct)     ((temp_direct) == UP || (temp_direct) == DOWN || (temp_direct) == LEFT || (temp_direct) == RIGHT || (temp_direct) == STOP)
#define RANDOM_ROW()                ((rand() % ((RANDOM_ROW_UPPER_MAX - RANDOM_ROW_UPPER_MIN) + 1)) + RANDOM_ROW_UPPER_MIN)
#define RANDOM_COL()                ((rand() % ((RANDOM_COL_UPPER_MAX - RANDOM_COL_UPPER_MIN) + 1)) + RANDOM_COL_UPPER_MIN)
#define GAMEOVER()                      \
    do                                  \
    {                                   \
        system("cls");                  \
        fprintf(stderr, "Game Over\n"); \
        exit(EXIT_FAILURE);             \
    } while (0);

static int snake_size;

static const char* srate_mode[3] =
{
    "SLOW",
    "MEDIUM",
    "FAST",
};

/**
 * @brief Create a game area object
 *
 * if allocates memory return non zero
 * if not allocates memory return NULL
 *
 * @return HAREA
 */
PRIVATE HAREA createGameArea(void) {
    HAREA area = (HAREA)calloc(1, HIGHT * WIDTH * sizeof(AREA));
    if (!area) {
        fprintf(stderr, "Out of Memory\n");
        return NULL;
    }

    size_t size = AREA_SIZE();
    while (size) {
        area[size - 1] = ' ';
        size--;
    }

    return area;
}

/**
 * @brief Create a snake object
 *
 * if allocates memory return non zero
 * if not allocates memory return NULL
 *
 * @return HSNAKE
 */
PRIVATE HSNAKE createSnake(void) {
    HSNAKE snake = calloc(1, sizeof(SNAKE) * HIGHT * WIDTH);
    if (!snake) {
        fprintf(stderr, "Out of Memory\n");
        return NULL;
    }
    snake[HEAD].stomach = PEACE;
    snake_size = 1;
    snake[HEAD].row = RANDOM_ROW();
    snake[HEAD].col = RANDOM_COL();
    return snake;
}

/**
 * @brief Create a baid object,
 *
 * if allocates memory return non zero
 * if not allocates memory return NULL
 *
 * @return HBAIT
 */
PRIVATE HBAIT createBait(void) {
    HBAIT bait = (HBAIT)calloc(1, sizeof(BAID));
    if (!bait) {
        fprintf(stderr, "Out of Memory\n");
        return NULL;
    }
    bait->row = RANDOM_ROW();
    bait->col = RANDOM_COL();
    return bait;
}

/**
 * @brief Create all objects
 *
 * @param area
 * @param snake
 * @param bait
 */
PUBLIC void createObjects(HAREA* area, HSNAKE* snake, HBAIT* bait) {
    *area = createGameArea();
    *snake = createSnake();
    *bait = createBait();
}

/**
 * @brief delete all objects
 *
 * @param area
 * @param snake
 * @param bait
 */
PUBLIC void deleteObjects(HAREA* area, HSNAKE* snake, HBAIT* bait) {
    free(*area);
    free(*snake);
    free(*bait);
}

/**
 * @brief Set the Rate object
 *
 * @param rate
 */
PUBLIC void setRate(Rate rate) {
    Sleep(rate);
}

/**
 * @brief Set the window object to area
 *
 * @param area
 */
PRIVATE void setArea(HAREA area) {
    for (int i = 0; i < AREA_SIZE(); ++i) {
        if (i / WIDTH < 1) // 0th row
            area[i] = '#';
        else if (!(i % WIDTH)) // 0th col
            area[i] = '#';
        else if ((i % WIDTH) == WIDTH - 1) // last row
            area[i] = '#';
        else if (!((i / ((HIGHT - 1) * WIDTH)) < 1)) // last col
            area[i] = '#';
        else
            area[i] = ' ';
    }
}

/**
 * @brief Set the snake object to area
 *
 * @param area
 * @param snake
 */
PRIVATE void setSnake(HAREA area, const HSNAKE snake) {
    for (int i = 0; i < snake_size; ++i)
        area[snake[i].row * WIDTH + snake[i].col] = snake[i].stomach;

    area[snake[0].row * WIDTH + snake[0].col] = HPEACE;
}

/**
 * @brief Set the baid object to area
 *
 * @param area
 * @param bait
 */
PRIVATE void setBaid(HAREA area, const HBAIT bait) {
    area[bait->row * WIDTH + bait->col] = FOOD;
}

/**
 * @brief Set all objects
 *
 * @param area
 * @param snake
 * @param bait
 */
PUBLIC void setObjects(HAREA* area, HSNAKE* snake, HBAIT* bait) {
    setArea(*area);
    setSnake(*area, *snake);
    setBaid(*area, *bait);
}

/**
 * @brief if pressed invalid key, update direct
 *
 * @param _direct
 */
PUBLIC void keyPressedOrNot(int* _direct) {
    if (_kbhit()) {
        int c = toupper(_getch());
        if (ISVALIDKEY(c)) {
            *_direct = c;
        }
    }
}

/**
 * @brief Makes the snake move
 *
 * @param direction
 * @param snake
 */
PUBLIC void move(int direction, HSNAKE snake) {
    for (int i = snake_size - 1; i > 0; --i) {
        snake[i].row = snake[i - 1].row;
        snake[i].col = snake[i - 1].col;
    }

    switch (direction) {
    case UP:
        snake[HEAD].row -= 1;
        if (snake[HEAD].row == 0) // is the up wall ?
            GAMEOVER();
        break;
    case DOWN:
        snake[HEAD].row += 1;
        if (snake[HEAD].row == HIGHT - 1) // is the down wall ?
            GAMEOVER();
        break;
    case LEFT:
        snake[HEAD].col -= 1;
        if (snake[HEAD].col == 0) // is the left wall ?
            GAMEOVER();
        break;
    case RIGHT:
        snake[HEAD].col += 1;
        if (snake[HEAD].col == WIDTH - 1) // is the rigt wall ?
            GAMEOVER();
        break;
    }
}

/**
 * @brief Throws the bait somewhere randomly.
 *
 * @param area
 * @param bait
 */
PRIVATE void randomBaid(HAREA area, HBAIT bait) {
    area[bait->row * WIDTH + bait->col] = ' ';
    bait->row = RANDOM_ROW();
    bait->col = RANDOM_COL();
    area[bait->row * WIDTH + bait->col] = FOOD;
}

/**
 * @brief Did it eat the bait ?
 *
 * @param area
 * @param snake
 * @param bait
 * @param dir Direction
 */
PUBLIC void eating(HAREA area, HSNAKE snake, HBAIT bait, Direction dir) {
    if (snake[HEAD].row == bait->row && snake[HEAD].col == bait->col) {

        randomBaid(area, bait);
        snake[snake_size].stomach = PEACE;

        switch (dir) {
        case UP:
            snake[snake_size].row = snake[snake_size - 1].row + 1;
            snake[snake_size].col = snake[snake_size - 1].col;
            break;
        case DOWN:
            snake[snake_size].row = snake[snake_size - 1].row - 1;
            snake[snake_size].col = snake[snake_size - 1].col;
            break;
        case LEFT:
            snake[snake_size].row = snake[snake_size - 1].row;
            snake[snake_size].col = snake[snake_size - 1].col + 1;
            break;
        case RIGHT:
            snake[snake_size].row = snake[snake_size - 1].row;
            snake[snake_size].col = snake[snake_size - 1].col - 1;
            break;
        }

        snake_size++;
    }
}

/**
 * @brief reset cursor position
 *
 */
PUBLIC void resetScreenPosition(void) {
    HANDLE handle;
    COORD position;
    handle = GetStdHandle(STD_OUTPUT_HANDLE);
    position.X = 0;
    position.Y = 0;
    SetConsoleCursorPosition(handle, position);
}

/**
 * @brief print the openin message to the console screen
 *
 */
PUBLIC void openingMessage(void) {
    printf("The Game Rules\n\n1.If you hit the walls it's game over\n2.Every bait is %d point\n3.Press the space key to stop\n\n", BAIT_POINT);
    printf("Press the any key to start...");

    int c;
    while (!_kbhit()) {
        if ((c = _getch()))
            break;
    }

    system("cls");
}

/**
 * @brief Get the Score object
 *
 */
PUBLIC size_t getScore(void) {
    return (snake_size - 1) * BAIT_POINT;
}

/**
 * @brief get to buffer from input buffer
 *
 * @param buff
 * @param size
 */
PUBLIC char* getsToBuffer(char* buff, size_t size) {
    if (!fgets(buff, size, stdin))
        return NULL;
    int i = 0;
    while (buff[i++] != '\n')
        ;
    buff[i - 1] = '\0';

    return buff;
}

/**
 * @brief print the area to the console screen
 *
 * @param area
 */
PUBLIC void printArea(const HAREA area) {
    for (int i = 0; i < AREA_SIZE(); ++i) {
        if (i != 0 && i % WIDTH == 0)
            printf("\n");
        printf("%c", area[i]);
    }
}

void printGameInfo(char* nick_name, size_t score, int rate_mode) {
    printf("Rate mode = %-6s\n\nPlayer = %-16s\nScore = %-10zu\n", srate_mode[rate_mode], nick_name, score);
}