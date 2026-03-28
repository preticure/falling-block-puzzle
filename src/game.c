#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "field.h"
#include "game.h"
#include "mino.h"
#include "score.h"

int gameState;

int buffer[BUFFER_SIZE];
int hold[HOLD_SIZE];

static int front = 0;
static int rear = 0;
static int count = 0;

static bool Enqueue(int i)
{
    if (count == BUFFER_SIZE)
    {
        return false;
    }
    buffer[rear] = i;
    rear = (rear + 1) % BUFFER_SIZE;
    count++;
    return true;
}

static int Dequeue()
{
    if (count == 0)
    {
        return -1; /* TODO: Errorにする */
    }
    int i = buffer[front];
    front = (front + 1) % BUFFER_SIZE;
    count--;
    return i;
}

static void Shuffle(int *array)
{
    int i = BAG_SIZE;
    do
    {
        int j = random() % i;
        i--;
        int t = array[i];
        array[i] = array[j];
        array[j] = t;
    } while (i > 1);
}

long DropSpeed()
{
    long speed[LEVEL_MAX] = {800000, 717000, 633000, 550000, 467000, 383000,
                             300000, 217000, 133000, 100000, 83000,  83000,
                             83000,  67000,  67000,  67000,  50000,  50000,
                             50000,  33000,  33000,  33000,  33000,  33000,
                             33000,  33000,  33000,  33000,  33000,  17000};
    return speed[Level() - 1];
}

static void SetBag()
{
    int bag[BAG_SIZE] = {D_I,    TRI_I,  TRI_L,  TETR_I, TETR_O,
                         TETR_S, TETR_Z, TETR_J, TETR_L, TETR_T};
    Shuffle(bag);
    for (int i = 0; i < BAG_SIZE; i++)
    {
        Enqueue(bag[i]);
    }
}

static void InitBuffer()
{
    SetBag();
    SetBag();
    hold[0] = -1;
}

/* 出現リストから次のミノを取り出す
 * 戻り値: SHAPE_INDEX
 * リストがBAG_SIZE以下になったら次の7種類をシャッフルして追加する
 */
static int NextShape()
{
    int index = Dequeue();
    if (count <= BAG_SIZE)
    {
        SetBag();
    }
    return index;
}

SHAPE NthShape(int n) { return shape[buffer[(front + n) % BUFFER_SIZE]]; }

void InitMino()
{
    int index = NextShape();
    SetStartPosition(index);
}

void Init()
{
    printf("\033[2J"); /* 画面全体をクリア */
    srandom(time(NULL));
    memset(field, 0, sizeof(field));

    InitBuffer();
    InitMino();
    InitAllScore();
}

void LandMino()
{
    WriteBlock(&mino);
    if (IsOutsideSafeArea())
    {
        gameState = GAMEOVER;
    }

    int lines = FullLinesCount(&mino);
    if (lines > 0)
    {
        AddClearLineScore(lines);
        ClearFullLines(&mino);
    }
    else
    {
        InitCombo();
    }

    InitMino();
    if (IsCollision(&mino))
    {
        gameState = GAMEOVER;
    }
}

void HoldMino()
{
    if (hold[0] == -1)
    {
        hold[0] = mino.shape.index;
        InitMino();
    }
    else
    {
        int temp = hold[0];
        hold[0] = mino.shape.index;
        SetStartPosition(temp);
    }
}

void HandleInput()
{
    const MINO lastmino = mino;
    switch (getchar())
    {
    case 'w':
    {
        int y = LandingY();
        mino.y = y;
        LandMino();
        AddHardDropScore(y - lastmino.y);
        break;
    }
    case 'a':
        mino.x--;
        if (IsCollision(&mino))
        {
            mino = lastmino;
        }
        break;
    case 's':
        mino.y++;
        if (IsCollision(&mino))
        {
            mino = lastmino;
        }
        else
        {
            AddSoftDropScore(1);
        }
        break;
    case 'd':
        mino.x++;
        if (IsCollision(&mino))
        {
            mino = lastmino;
        }
        break;
    case 'x':
        CWRotateMino();
        if (IsCollision(&mino))
        {
            mino = lastmino;
        }
        break;
    case 'z':
        CCWRotateMino();
        if (IsCollision(&mino))
        {
            mino = lastmino;
        }
        break;
    case 'c':
        HoldMino();
        break;
    case 'p':
        gameState = PAUSED;
        break;
    case 'q':
        gameState = QUIT;
        break;
    default:
        break;
    }
}

void HandlePausedInput()
{
    switch (getchar())
    {
    case 'p':
        gameState = PLAYING;
        break;
    case 'q':
        gameState = QUIT;
        break;
    default:
        break;
    }
}

void HandleGameOverInput()
{
    switch (getchar())
    {
    case 'q':
        gameState = QUIT;
        break;
    default:
        break;
    }
}
