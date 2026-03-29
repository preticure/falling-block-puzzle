#ifndef FIELD_H
#define FIELD_H

#include "mino.h"

#define FIELD_WIDTH (9)
#define FIELD_HEIGHT (21)

#define VISIBLE_START_ROW (3)

/* フィールドバッファ */
extern int field[FIELD_HEIGHT][FIELD_WIDTH];
/* スクリーンバッファ */
extern int screen[FIELD_HEIGHT][FIELD_WIDTH];

/* フィールドバッファにブロックを書き込む */
void WriteBlock(const MINO *const mino);

/* 揃ったライン数を返す（0〜4の範囲） */
int FullLinesCount(const MINO *const mino);

/* ラインクリア */
void ClearFullLines(const MINO *const mino);

#endif