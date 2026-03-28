CC 		= gcc
TARGET 	= build/main
SRCS 	= main.c game.c score.c terminal.c mino.c field.c
HDRS 	= $(wildcard $(INC)*.h)
INC 	= src/

$(TARGET): $(addprefix $(INC), $(SRCS))
	$(CC) $(addprefix $(INC), $(SRCS)) -o $(TARGET)

TEST_CFLAGS = -IUnity/src -Isrc
TEST_TARGET = build/test_runner
TEST_SRCS	= test/test_mino.c src/game.c src/mino.c Unity/src/unity.c

$(TEST_TARGET): $(TEST_SRCS)
	$(CC) $(TEST_CFLAGS) $(TEST_SRCS) -o $(TEST_TARGET)

run: $(TARGET)
	./$(TARGET)

format:
	clang-format -i $(addprefix $(INC), $(SRCS)) $(HDRS)

test: $(TEST_TARGET)
	./$(TEST_TARGET)