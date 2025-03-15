/*
defines.hpp - parentless

Defines log macros, utelizing spdlog. Defines
long types, such as AttatchmentRegistry.

*/


#pragma once

#include <spdlog/spdlog.h>
#include <entt/entt.hpp>
#include <nlohmann/json.hpp>
using nlohmann::json;

#include <fmt/format.h>
#include <unordered_map>
#include <functional>

namespace moss {

//////////////////////
//// -- Macros -- ////
//////////////////////

static int infoStep1 = 0;
static int infoStep2 = 0;
static int infoStep3 = 0;

#define ERROR(message, ...) spdlog::error("[{}]-[{}]-[{}]: {}", __FILE__, __FUNCTION__, __LINE__, fmt::format(message, __VA_ARGS__)); std::exit(0);
#define WARN(message, ...) spdlog::warn("[{}]-[{}]-[{}]: {}", __FILE__, __FUNCTION__, __LINE__, fmt::format(message, __VA_ARGS__))
#define DEB(message, ...) spdlog::debug("[{}]-[{}]-[{}]: {}", __FILE__, __FUNCTION__, __LINE__, fmt::format(message, __VA_ARGS__))
#define INFO_INIT(message, infoStep, ...) spdlog::info("{}.{}.{}\t >> {}", infoStep1, infoStep2, infoStep3, fmt::format(message, __VA_ARGS__)); \
    if (infoStep == 1) { infoStep1++; infoStep2 = 0; infoStep3 = 0; } \
    else if (infoStep == 2) { infoStep2++;  infoStep3 = 0; } \
    else if (infoStep == 3) { infoStep3++; }

#define ERROR_IF(statement, message, ...) if (statement) { \
    ERROR(message, __VA_ARGS__); \
}

#define WARN_IF(statement, message, ...) if (statement) { \
    WARN(message, __VA_ARGS__); \
}


/////////////////////
//// -- Types -- ////
/////////////////////
namespace types {

using AttachmentRegistry = std::unordered_map<std::string, std::function<void(entt::registry&, entt::entity&, const json&)>>;

} // types

} // moss
