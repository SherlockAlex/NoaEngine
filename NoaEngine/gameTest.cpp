//#define GAMETEST
#ifdef GAMETEST

#include "NoaEngine.h"

#define WIDTH 20
#define HEIGHT 20

static int headX, headY;
static float speed = 5;
static int fruitX, fruitY;
static int score;
static int gameOver;
static int snakeLength;
static int tailX[100], tailY[100];

static float delta;

static enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };
static enum Direction dir;

static void Draw();
static void Input();
static void Logic();

static void GameStart(void)
{
	//write your game init
    gameOver = false;
    dir = STOP;
    headX = WIDTH / 2;
    headY = HEIGHT / 2;
    fruitX = rand() % WIDTH;
    fruitY = rand() % HEIGHT;
    score = 0;

    delta = (float)WIDTH / (float)surfaceWidth;

}

static void GameUpdate(void) {
	//write your game loop
    Draw();
    Input();
    Logic();
}

SET_GAME_START(GameStart)
SET_GAME_LOOP(GameUpdate)
SET_GAME_WINDOW(1080 / 2, 1080 / 2, WindowMode)

static void Draw()
{

    for (int x = 0; x < surfaceWidth; x++)
    {
        for (int y = 0; y < surfaceHeight; y++)
        {
            int i = (float)y* delta;
            int j = (float)x* delta;

            //printf("%d\n",j);

            if (j == 0) {
                //printf("#"); // ×ó±ß½ç
                DrawPixel(x, y, WHITE);
            }
                

            if (i == headY && j == headX) {
                //printf("O"); // ÉßÍ·
                DrawPixel(x, y, GREEN);
            }
                
            else if (i == fruitY && j == fruitX) {
                //printf("F"); // Ë®¹û
                DrawPixel(x, y, RED);
            }
            else
            {
                bool printTail = false;

                for (int k = 0; k < snakeLength; k++)
                {
                    if (tailX[k] == j && tailY[k] == i)
                    {
                        //printf("o"); // ÉßÎ²
                        DrawPixel(x, y, GREEN);
                        printTail = true;
                    }
                }

                if (!printTail) {
                    //printf(" "); // ¿Õ¸ñ
                    DrawPixel(x, y, BLACK);
                }
            }

            //if (j == WIDTH - 1) {
            //    printf("#"); // ÓÒ±ß½ç
            //}
        }

        //printf("\n");
    }

    //for (int i = 0; i < WIDTH + 2; i++)
    //    printf("#"); // ÏÂ±ß½ç

    //printf("\n");
    printf("Score: %d\n", score);
}

static void Input()
{
    if (GetKeyHold(KeyA)) 
    {
        dir = LEFT;
        headX = headX - speed * deltaTime;
    }
    else if(GetKeyHold(KeyD))
    {
        dir = RIGHT;
        headX = headX + speed * deltaTime;
    }
    else if (GetKeyHold(KeyW))
    {
        dir = UP;
        headY = headY + speed * deltaTime;
    }
    else if (GetKeyHold(KeyS))
    {
        dir = DOWN;
        headY = headY - speed * deltaTime;
    }
    else if (GetKeyHold(KeyX))
    {
        gameOver = true;
    }

    /*switch (dir)
    {
    case LEFT:
        headX--;
        break;
    case RIGHT:
        headX++;
        break;
    case UP:
        headY--;
        break;
    case DOWN:
        headY++;
        break;
    }*/

}

static void Logic()
{
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = headX;
    tailY[0] = headY;

    for (int i = 1; i < snakeLength; i++)
    {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    

    // ¼ì²âÅö×²
    if (headX < 0 || headX >= WIDTH || headY < 0 || headY >= HEIGHT)
        gameOver = true;

    for (int i = 0; i < snakeLength; i++)
    {
        if (tailX[i] == headX && tailY[i] == headY)
            gameOver = true;
    }

    if (headX == fruitX && headY == fruitY)
    {
        score += 10;
        fruitX = rand() % WIDTH;
        fruitY = rand() % HEIGHT;
        snakeLength++;
    }
}

#endif // GAMETEST

