#pragma once

#include <string>

#include <nlohmann/json.hpp>

void ExpectKeyword(const nlohmann::json& node, const char* keyword);
const nlohmann::json& GetInstructionOperand(const nlohmann::json& node, const char* keyword);
std::string GetStringOperand(const nlohmann::json& node, const char* keyword);
