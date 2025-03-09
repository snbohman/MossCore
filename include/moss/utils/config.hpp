/*
utils/config.hpp - parentless

Implements utils::json with relevant helper functions.

*/

#pragma once

#include <moss/meta/libs.hpp>
#include <moss/meta/defines.hpp>
#include <fstream>

namespace moss::utils::config {

inline void readConfig(json& j, const char* configPath) {
    j = json::parse(std::fstream(configPath));
    if (j.empty()) {
        ERROR("Json config from \"{}\" read empty", configPath);
    }
}

} // moss::utils::json
