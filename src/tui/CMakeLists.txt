add_executable(tui)

target_sources(tui
    PRIVATE
        main.c
        terminal.c
)
target_link_libraries(tui
    PRIVATE
        core
)