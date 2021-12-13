#ifndef FC_H_SNAKE_GAME
#define FC_H_SNAKE_GAME

#include <stddef.h>

/*!< Object Like Macro Definitions */
#define HIGHT                       25
#define WIDTH                       50
#define HEAD                        0
#define BAIT_POINT                  10
#define FOOD                        64   // @
#define PEACE                       176 // o
#define HPEACE                      178
#define AREA_SIZE()                 (HIGHT * WIDTH)
#define RANDOM_ROW_UPPER_MAX        (HIGHT - 2)
#define RANDOM_ROW_UPPER_MIN        (1)
#define RANDOM_COL_UPPER_MAX        (WIDTH - 2)
#define RANDOM_COL_UPPER_MIN        (1)
#define STOP                        32   // ' '

#define RANDOMIZED() (srand((unsigned)time(NULL)))  // for random game 

typedef int* HAREA;
typedef int AREA;
typedef struct {
  int stomach;
  int row;
  int col;
} SNAKE, * HSNAKE;

typedef struct {
  int row;
  int col;
} BAID, * HBAIT;

typedef enum Direction {
  UP = 'W',
  DOWN = 'S',
  LEFT = 'A',
  RIGHT = 'D',
} Direction;

typedef enum Rate {
  SLOW = 40,
  MEDIUM = 20,
  FAST = 5,
} Rate;

void createObjects(HAREA*, HSNAKE*, HBAIT*);
void deleteObjects(HAREA*, HSNAKE*, HBAIT*);
void setObjects(HAREA*, HSNAKE*, HBAIT*);
void setRate(Rate);
void move(int, HSNAKE);
void eating(HAREA, HSNAKE, HBAIT, Direction);
void keyPressedOrNot(int*);
void resetScreenPosition(void);
void openingMessage(void);
char* getsToBuffer(char*, size_t);
size_t getScore(void);
void printArea(const HAREA);
void printGameInfo(char*, size_t, int);

#endif // !FC_H_SNAKE_GAME
