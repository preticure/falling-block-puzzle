#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

#include "mino.h"

#define BUFFER_SIZE (20)
#define VISIBLE_BUFFER (6) /* 出現リストの表示数 */

#define BAG_SIZE (10)

#define HOLD_SIZE (1)

/* ゲームステート */
enum
{
    PLAYING,
    PAUSED,
    GAMEOVER,
    QUIT /* TODO: いらなくなったら消す */
};

/* セル */
enum
{
    CELL_EMPTY,
    CELL_BLOCK,
    CELL_GHOST
};

/* ゲームステート */
extern int gameState;

/* 出現リスト */
extern int buffer[BUFFER_SIZE];

/* ホールド */
extern int hold[HOLD_SIZE];

/* 落下速度を返す */
long DropSpeed();

/* 出現リストからn番目の形状を返す */
SHAPE NthShape(int n);

/* ミノを初期化 */
void InitMino();

/* 初期化 */
void Init();

/* 着地シーケンス */
void LandMino();

/* アクティブなミノをホールドバッファに保持 */
void HoldMino();

/* 入力処理 */
void HandleInput();

/* 一時停止中の入力処理 */
void HandlePausedInput();

/* ゲームオーバー中の入力処理 */
void HandleGameOverInput();

#endif
