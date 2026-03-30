add_library(core)

target_sources(core
    PUBLIC
        game.c
        mino.c
        field.c
        score.c

    PUBLIC
        FILE_SET HEADERS
        FILES
            game.h
            mino.h
            field.h
            score.h
)