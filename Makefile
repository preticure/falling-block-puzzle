CC 			= gcc
CFLAGS 		= -Isrc/core -Isrc/tui

# TUI
TUI_TARGET 	= build/main
CORE_SRCS 	= src/core/game.c src/core/score.c src/core/mino.c src/core/field.c
TUI_SRCS 	= src/tui/main.c src/tui/terminal.c

$(TUI_TARGET): $(CORE_SRCS) $(TUI_SRCS) 
	$(CC) $(CFLAGS) $(CORE_SRCS) $(TUI_SRCS) -o $(TUI_TARGET)

# Test
TEST_TARGET = build/test_runner
TEST_CFLAGS = -IUnity/src -Isrc/core
TEST_SRCS	= test/test_mino.c $(CORE_SRCS) Unity/src/unity.c

$(TEST_TARGET): $(TEST_SRCS)
	$(CC) $(TEST_CFLAGS) $(TEST_SRCS) -o $(TEST_TARGET)

# Commands
run-tui: $(TUI_TARGET)
	./$(TUI_TARGET)

format:
	clang-format -i $(CORE_SRCS) $(TUI_SRCS) $(wildcard src/core/*.h) $(wildcard src/tui/*.h)

test: $(TEST_TARGET)
	./$(TEST_TARGET)