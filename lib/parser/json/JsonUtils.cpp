#include "parser/json/JsonUtils.hpp"

#include <string>

#include "error/Errors.hpp"

void ExpectKeyword(const nlohmann::json& node, const char* keyword) {
    if (!node.is_string() || node.get<std::string>() != keyword) {
        throw SyntaxError(std::string(keyword) + " must be encoded as a string");
    }
}

const nlohmann::json& GetInstructionOperand(const nlohmann::json& node, const char* keyword) {
    if (!node.is_object() || node.size() != 1 || !node.contains(keyword)) {
        throw SyntaxError(std::string(keyword) + " must be a single-field object");
    }
    return node.at(keyword);
}

std::string GetStringOperand(const nlohmann::json& node, const char* keyword) {
    const auto& operand = GetInstructionOperand(node, keyword);
    if (!operand.is_string()) {
        throw SyntaxError(std::string(keyword) + " operand must be a string");
    }
    return operand.get<std::string>();
}
