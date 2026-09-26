add_library(interpreter_lib
    value/Value.cpp
    binary_operator/BinaryOperatorRegistry.cpp
    instruction/Instruction.cpp
    parser/json/JsonParser.cpp
    parser/json/JsonUtils.cpp
    parser/json/instruction_parser/JsonInstructionParserRegistry.cpp
    parser/json/instruction_parser/ReadParser.cpp
    parser/json/instruction_parser/WriteParser.cpp
    parser/json/instruction_parser/LoadParser.cpp
    parser/json/instruction_parser/StoreParser.cpp
    parser/json/instruction_parser/ConstantParser.cpp
    parser/json/instruction_parser/BinaryOperationParser.cpp
    parser/json/instruction_parser/LabelParser.cpp
    parser/json/instruction_parser/JumpParser.cpp
    parser/json/instruction_parser/JumpZeroParser.cpp
    parser/json/instruction_parser/JumpNotZeroParser.cpp
    runtime/environment/Environment.cpp
    runtime/io/StreamInput.cpp
    runtime/io/StreamOutput.cpp
    runtime/Machine.cpp
    runtime/Interpreter.cpp
)

target_include_directories(interpreter_lib PUBLIC ${PROJECT_SOURCE_DIR}/lib)
target_link_libraries(interpreter_lib PUBLIC nlohmann_json::nlohmann_json)
