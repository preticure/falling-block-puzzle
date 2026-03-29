#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/time.h>
#include <termios.h>
#include <time.h>

#include "field.h"
#include "game.h"
#include "mino.h"
#include "score.h"
#include "terminal.h"

void DrawShape(SHAPE *shape, int row, int col)
{

    /* どのポリオミノも出現時の実高さは2行以下 */
    for (int y = 0; y < 2; y++)
    {
        printf("\033[%d;%dH", row + y, col); /* カーソルを移動 */
        for (int x = 0; x < SHAPE_WIDTH; x++)
        {
            printf(shape->pattern[y][x] ? "■" : "　");
        }
    }
}

void DrawShapeBuffer()
{
    printf("\033[1;35H"); /* カーソルを移動 */
    printf("NEXT");
    for (int i = 0; i < VISIBLE_BUFFER; i++)
    {
        SHAPE shape = NthShape(i);
        int row = (i * 3) + 2;
        DrawShape(&shape, row, 35);
    }
}

void DrawHold()
{
    printf("\033[1;1H"); /* カーソルを移動 */
    printf("HOLD");
    if (hold[0] >= 0)
    {
        DrawShape(&shape[hold[0]], 2, 1);
    }
    else
    {
        for (int y = 0; y < 2; y++)
        {
            printf("\033[%d;%dH", y + 2, 1); /* カーソルを移動 */
            for (int x = 0; x < SHAPE_WIDTH; x++)
            {
                printf("　");
            }
        }
    }
}

/* ステータス行を描画 */
void DrawStatusLine()
{
    switch (gameState)
    {
    case PAUSED:
        printf("PAUSED\n");
        break;
    case GAMEOVER:
        printf("GAMEOVER\n");
        break;
    default:
        for (int x = 0; x < FIELD_WIDTH + 2; x++)
        {
            printf("　");
        }
        printf("\n");
        break;
    }
}

void DrawScoreLine() { printf("SCORE: %d　LEVEL: %d\n", Score(), Level()); }

void DrawScreen()
{
    printf("\033[1;10H"); /* カーソルを左上にセット */

    memcpy(screen, field, sizeof(field));

    int ghostY = LandingY();

    /* ゴースト */
    for (int y = 0; y < mino.shape.height; y++)
    {
        for (int x = 0; x < mino.shape.width; x++)
        {
            if (mino.shape.pattern[y][x])
            {
                screen[ghostY + y][mino.x + x] = CELL_GHOST;
            }
        }
    }

    for (int y = 0; y < mino.shape.height; y++)
    {
        for (int x = 0; x < mino.shape.width; x++)
        {
            if (mino.shape.pattern[y][x])
            {
                screen[mino.y + y][mino.x + x] = CELL_BLOCK;
            }
        }
    }

    int row = 1;
    int col = 10;
    for (int y = VISIBLE_START_ROW; y < FIELD_HEIGHT; y++)
    {
        printf("□");
        for (int x = 0; x < FIELD_WIDTH; x++)
        {
            printf("%s", screen[y][x] == CELL_GHOST
                             ? "□"
                             : (screen[y][x] == CELL_BLOCK ? "■" : "　"));
        }
        printf("□");
        printf("\033[%d;%dH", ++row, col); /* カーソルを移動 */
    }
    for (int x = 0; x < FIELD_WIDTH + 2; x++)
    {
        printf("□");
    }
    printf("\033[%d;%dH", ++row, col); /* カーソルを移動 */
    DrawStatusLine();
    DrawScoreLine();
    DrawShapeBuffer();
    DrawHold();
}

int main()
{
    struct termios oldt, newt;
    TerminalSet(&oldt, &newt);

    Init();
    DrawScreen();

    fd_set rfd_set;

    while (1)
    {
        switch (gameState)
        {
        case PLAYING:
        {
            MINO lastmino = mino;

            struct timeval timeout;
            timeout.tv_sec = 0;
            timeout.tv_usec = DropSpeed();

            FD_ZERO(&rfd_set);
            FD_SET(STDIN_FILENO, &rfd_set);
            int retval =
                select(STDIN_FILENO + 1, &rfd_set, NULL, NULL, &timeout);

            if (retval > 0)
            {
                HandleInput();
            }
            else if (retval == 0)
            {
                mino.y++;
                if (IsCollision(&mino))
                {
                    mino = lastmino;
                    LandMino();
                }
            }
            DrawScreen();
            break;
        }
        case PAUSED:
            HandlePausedInput();
            DrawScreen();
            break;
        case GAMEOVER:
            HandleGameOverInput();
            DrawScreen();
            break;
        case QUIT:
            goto END;
        }
    }
END:
    TerminalReset(&oldt);
    return 0;
}
