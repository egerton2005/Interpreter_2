add_library(compiler
	Compiler.cpp
)

target_include_directories(compiler PUBLIC ${PROJECT_SOURCE_DIR}/lib)
target_link_libraries(compiler PUBLIC ast instruction)