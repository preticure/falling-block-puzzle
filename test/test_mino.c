#include <string.h>

#include "unity.h"
#include "mino.h"

void setUp(void)
{
    // set stuff up here
    memset(field, 0, sizeof(field));
    memset(&mino, 0, sizeof(mino));
}

void tearDown(void)
{
    // clean stuff up here
}

/* ミノの出現位置
     * x: フィールドの幅(FIELD_WIDTH) - ミノの幅(2,3,4) / 2
     * y: フィールドの見え始め行(VISIBLE_START_ROW) - 1 - ミノの最下ブロック行
     * 
     * ミノの幅は2,3,4の3種類なので、初期位置のx座標は3,4のいづれかになるはず
 */
void test_InitMinoWithShapeI_ShouldCenterHorizontally(void) {
    InitMinoWithShape(TETR_I); /* 幅4 */

    TEST_ASSERT_EQUAL_INT(3, mino.x); /* (10 - 4) / 2 = 3 */
    TEST_ASSERT_EQUAL_INT(1, mino.y); /* (3 - 1) - 1 = 1 */
}
void test_InitMinoWithShapeO_ShouldCenterHorizontally(void) {
    InitMinoWithShape(TETR_O); /* 幅2 */

    TEST_ASSERT_EQUAL_INT(4, mino.x); /* (10 - 2) / 2 = 4 */
    TEST_ASSERT_EQUAL_INT(1, mino.y); /* (3 - 1) - 1 = 1 */
}
void test_InitMinoWithShapeS_ShouldCenterHorizontally(void) {
    InitMinoWithShape(TETR_S); /* 幅3 */

    TEST_ASSERT_EQUAL_INT(3, mino.x); /* (10 - 3) / 2 = 3 */
    TEST_ASSERT_EQUAL_INT(1, mino.y); /* (3 - 1) - 1 = 1 */
}

void test_CWRotateMino_WithShapeI() {
    InitMinoWithShape(TETR_I);
    CWRotateMino();

    /*
        *   0 0 0 0      0 0 1 0
        *   1 1 1 1  →   0 0 1 0
        *   0 0 0 0      0 0 1 0
        *   0 0 0 0      0 0 1 0
     */
    int expected[SHAPE_HEIGHT][SHAPE_WIDTH] = {
        {0, 0, 1, 0},
        {0, 0, 1, 0},
        {0, 0, 1, 0},
        {0, 0, 1, 0}
    };
    TEST_ASSERT_EQUAL_INT_ARRAY(
        expected,
        mino.shape.pattern,
        SHAPE_HEIGHT * SHAPE_WIDTH
    );
}
void test_CWRotateMino_WithShapeZ() {
    InitMinoWithShape(TETR_Z);
    CWRotateMino();

    /*
        *   1 1 0      0 0 1
        *   0 1 1  →   0 1 1
        *   0 0 0      0 1 0
     */
    int expected[SHAPE_HEIGHT][SHAPE_WIDTH] = {
        {0, 0, 1},
        {0, 1, 1},
        {0, 1, 0},
    };
    TEST_ASSERT_EQUAL_INT_ARRAY(
        expected,
        mino.shape.pattern,
        SHAPE_HEIGHT * SHAPE_WIDTH
    );  
}

void test_CCWRotateMino_WithShapeI() {
    InitMinoWithShape(TETR_I);
    CCWRotateMino();

    /*
        *   0 0 0 0      0 1 0 0
        *   1 1 1 1  →   0 1 0 0
        *   0 0 0 0      0 1 0 0
        *   0 0 0 0      0 1 0 0
     */
    int expected[SHAPE_HEIGHT][SHAPE_WIDTH] = {
        {0, 1, 0, 0},
        {0, 1, 0, 0},
        {0, 1, 0, 0},
        {0, 1, 0, 0}
    };
    TEST_ASSERT_EQUAL_INT_ARRAY(
        expected,
        mino.shape.pattern,
        SHAPE_HEIGHT * SHAPE_WIDTH
    );
}
void test_CCWRotateMino_WithShapeZ() {
    InitMinoWithShape(TETR_Z);
    CCWRotateMino();

    /*
        *   1 1 0      0 1 0
        *   0 1 1  →   1 1 0
        *   0 0 0      1 0 0
     */
    int expected[SHAPE_HEIGHT][SHAPE_WIDTH] = {
        {0, 1, 0},
        {1, 1, 0},
        {1, 0, 0},
    };
    TEST_ASSERT_EQUAL_INT_ARRAY(
        expected,
        mino.shape.pattern,
        SHAPE_HEIGHT * SHAPE_WIDTH
    );  
}

void test_FixMino_ShouldSetBlocksInField() {
    InitMinoWithShape(TETR_O);
    mino.x = 5;
    mino.y = 21;

    FixMino();

    /*      expected:
        0 0 0 0 0 0 0 0 0 0 
        0 0 0 0 0 0 0 0 0 0
        0 0 0 0 0 0 0 0 0 0
        0 0 0 0 0 1 1 0 0 0
        0 0 0 0 0 1 1 0 0 0
        ...................
     */
    int expected[FIELD_HEIGHT][FIELD_WIDTH];
    memset(expected, 0, sizeof(expected));
    expected[21][5] = 1;
    expected[21][6] = 1;
    expected[22][5] = 1;
    expected[22][6] = 1;
    TEST_ASSERT_EQUAL_INT_ARRAY(
        expected,
        field,
        FIELD_HEIGHT * FIELD_WIDTH
    );
}

void test_IsCollision_ShouldReturnTrueWhenMinoIsOutOfBounds(void) {
    for (int i = 0; i < BAG_SIZE; i++) {
        InitMinoWithShape(i);
        mino.x = -10;
        mino.y = -10; /* 明らかにフィールドの範囲外 */
        TEST_ASSERT_TRUE(IsCollision(&mino));
    }
}
void test_IsCollision_ShouldReturnTrueWhenMinoHitsBlock(void) {
    for (int i = 0; i < BAG_SIZE; i++) {
        for (int y = VISIBLE_START_ROW; y < FIELD_HEIGHT; y++) {
            for (int x = 0; x < FIELD_WIDTH; x++) {
                field[y][x] = 1; /* フィールドがブロックで埋め尽くされている */
            }
        }
        InitMinoWithShape(i);
        mino.y = VISIBLE_START_ROW;
        TEST_ASSERT_TRUE(IsCollision(&mino));
    }
}
void test_IsCollision_ShouldReturnFalseWhenNoCollision(void) {
    for (int i = 0; i < BAG_SIZE; i++) {
        memset(field, 0, sizeof(field)); /* どこにもブロックがない */
        InitMinoWithShape(i);
        TEST_ASSERT_FALSE(IsCollision(&mino));
    }
}

void test_IsOutsideSafeArea_ShouldReturnTrueWhenAllBlocksAreAboveVisibleRow(void) {
    for (int i = 0; i < BAG_SIZE; i++) {
        InitMinoWithShape(i);
        mino.y = -10; /* 明らかにVISIBLE_START_ROW(3)より高い位置にある */
        TEST_ASSERT_TRUE(IsOutsideSafeArea());
    }
}
void test_IsOutsideSafeArea_ShouldReturnFalseWhenAnyBlockIsWithinVisibleArea(void) {
    for (int i = 0; i < BAG_SIZE; i++) {
        InitMinoWithShape(i);
        mino.y = VISIBLE_START_ROW -1; /* ミノの高さは2以上なので、2行目以降はVISIBLE_START_ROW以内 */
        TEST_ASSERT_FALSE(IsOutsideSafeArea());
    }
}

void test_FindAndClearFullLines_ShouldClearAllFullLines(void) {
    int y1 = 10;
    int y2 = 11;

    for (int i = 0; i < BAG_SIZE; i++) {
        memset(field, 0, sizeof(field));

        InitMinoWithShape(i);
        mino.y = y1;

        for (int x = 0; x < FIELD_WIDTH; x++) {
            field[y1][x] = 1;
            field[y2][x] = 1;
        }
        field[y1 - 1][0] = 1; /* y1のひとつ上にブロックがある */

        FindAndClearFullLines();

        int expectedy1[FIELD_WIDTH];
        int expectedy2[FIELD_WIDTH];
        memset(expectedy1, 0, sizeof(expectedy1));
        memset(expectedy2, 0, sizeof(expectedy2));
        expectedy2[0] = 1; /* y1とy2のブロックが消えて、y1-1のブロックは2行分移動しているはず */
        TEST_ASSERT_EQUAL_INT_ARRAY(
            expectedy1,
            field[y1],
            FIELD_WIDTH
        );
        TEST_ASSERT_EQUAL_INT_ARRAY(
            expectedy2,
            field[y2],
            FIELD_WIDTH
        );
    }
}

void test_LandingY_WithShapeI_ShouldReturnCorrectLandingY(void) {
    memset(field, 0, sizeof(field));
    InitMinoWithShape(TETR_I);
    mino.x = 3;
    mino.y = 0;
    TEST_ASSERT_EQUAL_INT(21, LandingY());
}

int main(void) {
    UNITY_BEGIN();

    /* InitMinoWithShape() */
    RUN_TEST(test_InitMinoWithShapeI_ShouldCenterHorizontally);
    RUN_TEST(test_InitMinoWithShapeS_ShouldCenterHorizontally);
    RUN_TEST(test_InitMinoWithShapeO_ShouldCenterHorizontally);

    /* CWRotateMino() */
    RUN_TEST(test_CWRotateMino_WithShapeI);
    RUN_TEST(test_CWRotateMino_WithShapeZ);

    /* CCWRotateMino() */
    RUN_TEST(test_CCWRotateMino_WithShapeI);
    RUN_TEST(test_CCWRotateMino_WithShapeZ);

    /* FixMino() */
    RUN_TEST(test_FixMino_ShouldSetBlocksInField);

    /* IsCollision() */
    RUN_TEST(test_IsCollision_ShouldReturnTrueWhenMinoIsOutOfBounds);
    RUN_TEST(test_IsCollision_ShouldReturnTrueWhenMinoHitsBlock);
    RUN_TEST(test_IsCollision_ShouldReturnFalseWhenNoCollision);

    /* IsOutsideSafeArea() */
    RUN_TEST(test_IsOutsideSafeArea_ShouldReturnTrueWhenAllBlocksAreAboveVisibleRow);
    RUN_TEST(test_IsOutsideSafeArea_ShouldReturnFalseWhenAnyBlockIsWithinVisibleArea);

    /* FindAndClearFullLines() */
    RUN_TEST(test_FindAndClearFullLines_ShouldClearAllFullLines);

    /* LandingY() */
    RUN_TEST(test_LandingY_WithShapeI_ShouldReturnCorrectLandingY);

    return UNITY_END();
}