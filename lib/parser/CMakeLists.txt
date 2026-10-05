add_library(text_parser
	Parser.cpp
	statement_parser/StatementParserRegistry.cpp
	statement_parser/StatementParsers.cpp
)

target_include_directories(text_parser PUBLIC ${PROJECT_SOURCE_DIR}/lib)
target_link_libraries(text_parser PUBLIC interpreter_lib)