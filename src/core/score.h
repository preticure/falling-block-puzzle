#ifndef SCORE_H
#define SCORE_H

#define INITIAL_LEVEL (1)
#define INITIAL_COMBO (-1)

#define LINES_PER_LEVEL (10)
#define LEVEL_MAX (30)

// clang-format off
enum
{
    SCORE_SINGLE    = 100,  /* 1ラインクリア */
    SCORE_DOUBLE    = 300,  /* 2ラインクリア */
    SCORE_TRIPLE    = 500,  /* 3ラインクリア */
    SCORE_QUAD      = 800,  /* 4ラインクリア */
    SCORE_SOFTDROP  = 1,    /* ソフトドロップ加算 */
    SCORE_HARDDROP  = 2,    /* ハードドロップ加算 */
    SCORE_COMBO     = 50    /* コンボ加算 */
};
// clang-format on

/* 現在のスコアを返す */
int Score();

/* 現在のレベルを返す */
int Level();

/* コンボ数をリセット（初期値は-1） */
void InitCombo();

/* スコア,レベル,クリアライン数,コンボ数を初期化 */
void InitAllScore();

/* ラインクリアによるスコアを加算
 * c: クリアしたライン数
 */
void AddClearLineScore(int c);

/* ソフトドロップアクションによるスコアを加算 */
void AddSoftDropScore(int c);

/* ハードドロップアクションによるスコアを加算 */
void AddHardDropScore(int c);

#endif