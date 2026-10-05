add_library(lexer
    Position.cpp
    Lexeme.cpp
    Cursor.cpp
    Lexer.cpp
)

target_include_directories(lexer PUBLIC ${PROJECT_SOURCE_DIR}/lib)