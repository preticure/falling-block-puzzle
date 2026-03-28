#include "score.h"

static int score;
static int combo;
static int level;
static int totalLines;

int Score() { return score; }

int Level() { return level; }

static void AddComboScore()
{

    score += SCORE_COMBO * combo;
    combo++;
}

void InitCombo() { combo = INITIAL_COMBO; }

void InitAllScore()
{
    score = 0;
    level = INITIAL_LEVEL;
    totalLines = 0;
    InitCombo();
}

static void AddTotalLines(int c)
{
    totalLines += c;
    level = (totalLines / LINES_PER_LEVEL) + 1;

    if (level > LEVEL_MAX)
    {
        level = LEVEL_MAX;
    }
}

void AddClearLineScore(int c)
{
    switch (c)
    {
    case 1:
        score += SCORE_SINGLE * level;
        break;
    case 2:
        score += SCORE_DOUBLE * level;
        break;
    case 3:
        score += SCORE_TRIPLE * level;
        break;
    case 4:
        score += SCORE_QUAD * level;
        break;
    default:
        return;
    }
    AddComboScore();
    AddTotalLines(c);
}

void AddSoftDropScore(int c) { score += SCORE_SOFTDROP * c; }

void AddHardDropScore(int c) { score += SCORE_HARDDROP * c; }