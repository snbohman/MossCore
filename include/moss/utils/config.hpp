/*
utils/config.hpp

Implements utils::config with relevant reading functions.

*/

#pragma once

#include <moss/meta/libs.hpp>
#include <moss/meta/defines.hpp>
#include <fstream>

namespace moss::utils::config {

inline void readConfig(json& j, const char* config, const char* dataDir="data") {
    static const char* dataDirectory = std::string(dataDir) == "data" ? "data" : dataDir;
    const std::string configPath = fmt::format("{}/{}", dataDirectory, configPath);

    j = json::parse(std::fstream(configPath));
    ERROR_IF(j.empty(), "Json config from path \"{}\" found empty", configPath);
}

} // moss::utils::json
