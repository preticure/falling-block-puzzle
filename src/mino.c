#include <stdlib.h>

#include "field.h"
#include "mino.h"

// clang-format off
SHAPE shape[ARRAY_SIZE] = {
    /* D_I（ドミノ） */
    {
        D_I, 2, 2,
        {
            {0, 0},
            {1, 1}
        }
    },
    /* TRI_I（トリオミノ I型） */
    {
        TRI_I, 3, 3,
        {
            {0, 0, 0},
            {1, 1, 1},
            {0, 0, 0}
        }
    },
    /* TRI_L（トリオミノ L型） */
    {
        TRI_L, 2, 2,
        {
            {0, 1},
            {1, 1}
        }
    },
    /* TETR_I（テトロミノ I型） */
    {
        TETR_I, 4, 4,
        {
            {0, 0, 0, 0},
            {1, 1, 1, 1},
            {0, 0, 0, 0},
            {0, 0, 0, 0}
        }
    },
    /* TETR_O（テトロミノ O型） */
    {
        TETR_O, 2, 2,
        {
            {1, 1},
            {1, 1}
        }
    },
    /* TETR_S（テトロミノ S型） */
    {
        TETR_S, 3, 3,
        {
            {0, 1, 1},
            {1, 1, 0},
            {0, 0, 0}
        }
    },
    /* TETR_Z（テトロミノ Z型） */
    { 
        TETR_Z, 3, 3,
        {
            {1, 1, 0},
            {0, 1, 1},
            {0, 0, 0}
        }
    },
    /* TETR_J（テトロミノ J型） */
    {
        TETR_J, 3, 3,
        {
            {1, 0, 0},
            {1, 1, 1},
            {0, 0, 0}
        }
    },
    /* TETR_L（テトロミノ L型） */
    {
        TETR_L, 3, 3,
        {
            {0, 0, 1},
            {1, 1, 1},
            {0, 0, 0}
        }
    },
    /* TETR_T（テトロミノ T型） */
    {
        TETR_T, 3, 3,
        {
            {0, 1, 0},
            {1, 1, 1},
            {0, 0, 0}
        }
    }
};
// clang-format on

MINO mino;

/* 形状の最下ブロック行を返す
 * 計算で求めているが、どの形状でも2を返すため、定数にしても良いかも */
static int ShapeBottomRow()
{
    int bottom = 0;
    for (int y = 0; y < mino.shape.height; y++)
    {
        for (int x = 0; x < mino.shape.width; x++)
        {
            if (mino.shape.pattern[y][x])
            {
                bottom = y;
            }
        }
    }
    return bottom;
}

void SetStartPosition(int i)
{
    mino.shape = shape[i];
    mino.x = (FIELD_WIDTH - mino.shape.width) / 2;
    mino.y = (VISIBLE_START_ROW - 1) - ShapeBottomRow();
}

void CWRotateMino()
{
    const MINO temp = mino;
    for (int y = 0; y < mino.shape.height; y++)
    {
        for (int x = 0; x < mino.shape.width; x++)
        {
            mino.shape.pattern[y][x] =
                temp.shape.pattern[temp.shape.width - x - 1][y];
        }
    }
}

void CCWRotateMino()
{
    const MINO temp = mino;
    for (int y = 0; y < mino.shape.height; y++)
    {
        for (int x = 0; x < mino.shape.width; x++)
        {
            mino.shape.pattern[y][x] =
                temp.shape.pattern[x][temp.shape.height - y - 1];
        }
    }
}

bool IsCollision(const MINO *const target)
{
    for (int y = 0; y < target->shape.height; y++)
    {
        for (int x = 0; x < target->shape.width; x++)
        {
            if (target->shape.pattern[y][x])
            {
                if (target->y + y >= FIELD_HEIGHT || target->x + x < 0 ||
                    target->x + x >= FIELD_WIDTH)
                {
                    return true;
                }
                if (field[target->y + y][target->x + x])
                {
                    return true;
                }
            }
        }
    }
    return false;
}

bool IsOutsideSafeArea()
{
    for (int y = 0; y < mino.shape.height; y++)
    {
        for (int x = 0; x < mino.shape.width; x++)
        {
            if (mino.shape.pattern[y][x])
            {
                if (mino.y + y >= VISIBLE_START_ROW)
                {
                    return false;
                }
            }
        }
    }
    return true;
}

int LandingY()
{
    MINO temp = mino;
    while (!IsCollision(&temp))
    {
        temp.y++;
    }
    return temp.y - 1;
}
