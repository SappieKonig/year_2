#include <iostream>
#include "config.h"

bool read_config_file(const std::filesystem::path& config_path, toml::table& config) {
    toml::parse_result result = toml::parse_file(config_path.string());
    if (!result) {
        std::cerr << "Parsing failed:\n" << result.error() << "\n";
        return false;
    }

    config = std::move(result.table());
    return true;
}