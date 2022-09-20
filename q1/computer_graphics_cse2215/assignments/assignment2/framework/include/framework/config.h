#pragma once

#define TOML_EXCEPTIONS 0
#include <toml/toml.hpp>
#include <filesystem>

bool read_config_file(const std::filesystem::path& config_path, toml::table& config);