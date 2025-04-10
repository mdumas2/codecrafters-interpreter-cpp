#pragma once

#include <vector>
#include <expected>
#include <string>

std::expected<std::vector<std::string>, int> tokenizer(std::string& file_content);