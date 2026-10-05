add_library(binary_operator
    BinaryOperatorRegistry.cpp
)

target_include_directories(binary_operator PUBLIC ${PROJECT_SOURCE_DIR}/lib)
target_link_libraries(binary_operator PUBLIC value error Microsoft.GSL::GSL)