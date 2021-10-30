#include <stdlib.h>  // srand(), rand()
#include <stdio.h>
#include <conio.h>   // kbhit(), _getch()
#include <ctype.h>   // toupper()
#include <time.h>    // time()
#include <Windows.h> // Sleep()

#include "snake_game.h"

#define PUBLIC 
#define PRIVATE static

/*!< Object Like Macro Definitions */
#define HIGHT                       25
#define WIDTH                       50
#define HEAD                        0
#define BAIT_POINT                  10
#define STOP                        32   // ' '
#define FOOD                        64   // @
#define PEACE                       111 // o
#define AREA_SIZE()                 (HIGHT * WIDTH)
#define RANDOM_ROW_UPPER_MAX        (HIGHT - 2)
#define RANDOM_ROW_UPPER_MIN        (1)
#define RANDOM_COL_UPPER_MAX        (WIDTH - 2)
#define RANDOM_COL_UPPER_MIN        (1)

/*!< Functional Like Macro Definitions */
#define RANDOM_ROW()                ((rand() % ((RANDOM_ROW_UPPER_MAX - RANDOM_ROW_UPPER_MIN) + 1)) + RANDOM_ROW_UPPER_MIN)
#define RANDOM_COL()                ((rand() % ((RANDOM_COL_UPPER_MAX - RANDOM_COL_UPPER_MIN) + 1)) + RANDOM_COL_UPPER_MIN)
#define ISVALIDKEY(temp_direct)     ((temp_direct) == UP || (temp_direct) == DOWN || (temp_direct) == LEFT || (temp_direct) == RIGHT || (temp_direct) == STOP)

#define GAMEOVER()                      \
    do                                  \
    {                                   \
        system("cls");                  \
        fprintf(stderr, "Game Over\n"); \
        exit(EXIT_FAILURE);             \
    } while (0);

#define KEY_PRESSED_OR_NOT()       \
    if (_kbhit())                  \
    {                              \
        int c = toupper(_getch()); \
        if (ISVALIDKEY(c))         \
        {                          \
            _direct = c;           \
        }                          \
    }

static int snake_size;

typedef int* HAREA;
typedef int AREA;

typedef enum Direction
{
    UP = 'W',
    DOWN = 'S',
    LEFT = 'A',
    RIGHT = 'D',
} Direction;

typedef struct
{
    int stomach;
    int row;
    int col;
} SNAKE, * HSNAKE;

typedef struct
{
    int row;
    int col;
} BAID, * HBAIT;

/**
 * @brief Create a game area object
 * 
 * if allocates memory return non zero
 * if not allocates memory return NULL
 * 
 * @return HAREA
 */
PRIVATE HAREA create_game_area(void)
{
    HAREA area = (HAREA)malloc(HIGHT * WIDTH * sizeof(AREA));
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
PRIVATE HSNAKE create_snake(void)
{
    HSNAKE snake = malloc(sizeof(SNAKE) * HIGHT * WIDTH);
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
PRIVATE HBAIT create_bait(void)
{
    HBAIT bait = (HBAIT)malloc(sizeof(BAID));
    if (!bait) {
        fprintf(stderr, "Out of Memory\n");
        return NULL;
    }
    bait->row = RANDOM_ROW();
    bait->col = RANDOM_COL();
    return bait;
}

/**
 * @brief Set the window object to area
 *
 * @param area
 */
PRIVATE void set_area(HAREA area)
{
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
PRIVATE void set_snake(HAREA area, const HSNAKE snake)
{
    for (int i = 0; i < snake_size; ++i)
        area[snake[i].row * WIDTH + snake[i].col] = snake[i].stomach;
}

/**
 * @brief Set the baid object to area
 *
 * @param area
 * @param bait
 */
PRIVATE void set_baid(HAREA area, const HBAIT bait)
{
    area[bait->row * WIDTH + bait->col] = FOOD;
}

/**
 * @brief if it is itself '1', if not '0'
 *
 * @param snake
 * @return int
 */
PRIVATE int is_yours(const HSNAKE snake)
{
    for (int i = 1; i < snake_size; ++i)
        if (snake[HEAD].row == snake[i].row && snake[HEAD].col == snake[i].col)
            return 1;

    return 0;
}

/**
 * @brief Makes the snake move
 *
 * @param direction
 * @param snake
 */
PRIVATE void move(int direction, HSNAKE snake)
{
    for (int i = snake_size - 1; i > 0; --i) {
        snake[i].row = snake[i - 1].row;
        snake[i].col = snake[i - 1].col;
    }

    switch (direction) {
    case UP:
        snake[HEAD].row -= 1;
        if ((snake[HEAD].row == 0) || is_yours(snake)) // is the up wall ?
            GAMEOVER();
        break;
    case DOWN:
        snake[HEAD].row += 1;
        if ((snake[HEAD].row == HIGHT - 1) || is_yours(snake)) // is the down wall ?
            GAMEOVER();
        break;
    case LEFT:
        snake[HEAD].col -= 1;
        if ((snake[HEAD].col == 0) || is_yours(snake)) // is the left wall ?
            GAMEOVER();
        break;
    case RIGHT:
        snake[HEAD].col += 1;
        if ((snake[HEAD].col == WIDTH - 1) || is_yours(snake)) // is the rigt wall ?
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
PRIVATE void random_baid(HAREA area, HBAIT bait)
{
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
PRIVATE void eating(HAREA area, HSNAKE snake, HBAIT bait, Direction dir)
{
    if (snake[HEAD].row == bait->row && snake[HEAD].col == bait->col) {

        random_baid(area, bait);
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
 * @brief Create all objects
 *
 * @param area
 * @param snake
 * @param bait
 */
PRIVATE void create_objects(HAREA* area, HSNAKE* snake, HBAIT* bait)
{
    *area = create_game_area();
    *snake = create_snake();
    *bait = create_bait();
}

/**
 * @brief Set all objects
 *
 * @param area
 * @param snake
 * @param bait
 */
PRIVATE void set_objects(HAREA* area, HSNAKE* snake, HBAIT* bait)
{
    set_area(*area);
    set_snake(*area, *snake);
    set_baid(*area, *bait);
}

/**
 * @brief delete all objects
 *
 * @param area
 * @param snake
 * @param bait
 */
PRIVATE void delete_objects(HAREA* area, HSNAKE* snake, HBAIT* bait)
{
    free(*area);
    free(*snake);
    free(*bait);
}

PRIVATE void print_area(const HAREA area)
{
    for (int i = 0; i < AREA_SIZE(); ++i) {
        if (i != 0 && i % WIDTH == 0)
            printf("\n");
        printf("%c", area[i]);
    }

    system("cls");
}

PUBLIC void opening_message(void)
{
    printf("The Game Rules\n\n1.If you hit the walls it's game over\n2.If you eat yourself it's game over\n3.Every bait is %d point\n4.Press the space key to stop\n\n", BAIT_POINT);
    printf("Press the any key to start...");

    int c;
    while (!_kbhit()) {
        if (c = _getch())
            break;
    }

    system("cls");
}

PUBLIC size_t get_score(void)
{
    return (snake_size - 1) * BAIT_POINT;
}

/**
 * @brief call the function to start the game
 *
 */
PUBLIC void play_snake_game(const char* nick_name)
{
    HAREA area = NULL;
    HSNAKE snake = NULL;
    HBAIT bait = NULL;
    int _direct = STOP;

    create_objects(&area, &snake, &bait);
    for (;;) {
        set_objects(&area, &snake, &bait);
        KEY_PRESSED_OR_NOT();
        move(_direct, snake);
        eating(area, snake, bait, _direct);
        print_area(area);
        printf("Player = %s    Score = %d\n", nick_name, get_score());
    }
    delete_objects(&area, &snake, &bait);
}

