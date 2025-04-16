/**
 * @file utils/config.hpp
 * @brief Implements utils::config with relevant reading functions.
 */

#pragma once

#include <moss/meta/libs.hpp>
#include <moss/meta/defines.hpp>
#include <fstream>

namespace moss::utils::config {

inline void readConfig(json& j, const std::string& config, const std::string& dataDir="data") {
    static std::string dataDirectory = dataDir;
    if (dataDirectory == "data" && dataDir != "data") {
        dataDirectory = dataDir;
    }

    const std::string configPath = fmt::format("{}/{}", dataDirectory, config);

    try {
        j = json::parse(std::fstream(configPath));
    } catch (std::exception e) {
        M_ERROR_IF(j.empty(), "Json config from path \"{}\" found empty", configPath);
    }
}

} // moss::utils::json
