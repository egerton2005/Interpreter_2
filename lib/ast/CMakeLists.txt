add_library(ast
    expression/BinOp.cpp
    expression/Const.cpp
    expression/Var.cpp

    statement/Assign.cpp
    statement/Block.cpp
    statement/DoWhile.cpp
    statement/If.cpp
    statement/Read.cpp
    statement/Skip.cpp
    statement/While.cpp
    statement/Write.cpp
)

target_include_directories(ast PUBLIC ${PROJECT_SOURCE_DIR}/lib)
target_link_libraries(ast PUBLIC value error binary_operator)