#include <stdbool.h>

#include "field.h"
#include "mino.h"

int field[FIELD_HEIGHT][FIELD_WIDTH];
int screen[FIELD_HEIGHT][FIELD_WIDTH];

void WriteBlock(const MINO *const mino)
{
    for (int y = 0; y < mino->shape.height; y++)
    {
        for (int x = 0; x < mino->shape.width; x++)
        {
            if (mino->shape.pattern[y][x])
            {
                field[mino->y + y][mino->x + x] = 1;
            }
        }
    }
}

/* 指定されたy行がブロックで埋まっているか判定 */
static bool IsLineFull(int y)
{
    for (int x = 0; x < FIELD_WIDTH; x++)
    {
        if (!field[y][x])
        {
            return false;
        }
    }
    return true;
}

int FullLinesCount(const MINO *const mino)
{
    int count = 0;
    for (int y = mino->y + mino->shape.height - 1; y >= 0; y--)
    {
        if (IsLineFull(y))
        {
            count++;
        }
    }
    return count;
}

static void ClearBlock(int y)
{
    for (int x = 0; x < FIELD_WIDTH; x++)
    {
        field[y][x] = 0;
    }
    for (int yy = y - 1; yy >= 0; yy--)
    {
        for (int x = 0; x < FIELD_WIDTH; x++)
        {
            field[yy + 1][x] = field[yy][x];
        }
    }
}

void ClearFullLines(const MINO *const mino)
{
    for (int y = mino->y + mino->shape.height - 1; y >= 0; y--)
    {
        if (IsLineFull(y))
        {
            ClearBlock(y);
            y++; /* 上の行もチェックするためにyを増やす */
        }
    }
}
