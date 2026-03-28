#ifndef MINO_H
#define MINO_H

#include <stdbool.h>

#define SHAPE_WIDTH (4)
#define SHAPE_HEIGHT (4)

/* ポリオミノの種類 */
enum
{
    D_I,       /* ドミノ（2ブロック）        */
    TRI_I,     /* トリオミノ I型（3ブロック） */
    TRI_L,     /* トリオミノ L型（3ブロック） */
    TETR_I,    /* テトロミノ I型（4ブロック） */
    TETR_O,    /* テトロミノ O型（4ブロック） */
    TETR_S,    /* テトロミノ S型（4ブロック） */
    TETR_Z,    /* テトロミノ Z型（4ブロック） */
    TETR_J,    /* テトロミノ J型（4ブロック） */
    TETR_L,    /* テトロミノ L型（4ブロック） */
    TETR_T,    /* テトロミノ T型（4ブロック） */
    ARRAY_SIZE /* 種類数（= 10） */
} SHAPE_INDEX;

/* ポリオミノ形状 */
typedef struct
{
    int index, width, height;
    int pattern[SHAPE_HEIGHT][SHAPE_WIDTH]; /* 0: 空, 1: ブロック */
} SHAPE;

/* アクティブなミノ */
typedef struct
{
    int x, y;
    SHAPE shape;
} MINO;

/* ポリオミノ形状 */
extern SHAPE shape[ARRAY_SIZE];

/* アクティブなミノ */
extern MINO mino;

/* ミノを開始位置にセット
 * i: SHAPE_INDEX
 * x: 中央（左丸め）
 * y: 形状の最下ブロックが19行目になるよう配置
 */
void SetStartPosition(int i);

/* ミノを時計回りに回転 */
void CWRotateMino();

/* ミノを反時計回りに回転 */
void CCWRotateMino();

/* ミノの衝突判定 */
bool IsCollision(const MINO *const target);

/* ミノが生存圏外に置かれたかを判定
 * どこか1ブロックでもVISIBLE_START_ROW以降にあればfalseを返す */
bool IsOutsideSafeArea();

/* ミノが着地するy座標を返す */
int LandingY();

#endif
