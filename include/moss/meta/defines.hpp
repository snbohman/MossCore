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

extern int infoStep1;
extern int infoStep2;
extern int infoStep3;
#define INFO_INIT(message, infoStep, ...) \
    if (infoStep == 1) { infoStep1++; infoStep2 = 0; infoStep3 = 0; } \
    else if (infoStep == 2) { infoStep2++;  infoStep3 = 0; } \
    else if (infoStep == 3) { infoStep3++; } \
    spdlog::info("[{}].[{}].[{}]\t >> {}", infoStep1, infoStep2, infoStep3, fmt::format(message, __VA_ARGS__));

#define INFO_INITF(message, infoStep) \
    if (infoStep == 1) { infoStep1++; infoStep2 = 0; infoStep3 = 0; } \
    else if (infoStep == 2) { infoStep2++;  infoStep3 = 0; } \
    else if (infoStep == 3) { infoStep3++; } \
    spdlog::info("[{}].[{}].[{}]\t >> {}", infoStep1, infoStep2, infoStep3, message); \


#define ERROR(message, ...) spdlog::error("[{}]-[{}]-[{}]: {}", __FILE__, __FUNCTION__, __LINE__, fmt::format(message, __VA_ARGS__)); std::exit(0);
#define WARN(message, ...) spdlog::warn("[{}]-[{}]-[{}]: {}", __FILE__, __FUNCTION__, __LINE__, fmt::format(message, __VA_ARGS__))
#define DEB(message, ...) spdlog::debug("[{}]-[{}]-[{}]: {}", __FILE__, __FUNCTION__, __LINE__, fmt::format(message, __VA_ARGS__))
#define INFO(message, ...) spdlog::info("[{}]-[{}]-[{}]: {}", __FILE__, __FUNCTION__, __LINE__, fmt::format(message, __VA_ARGS__))

#define ERRORF(message) spdlog::error("[{}]-[{}]-[{}]: {}", __FILE__, __FUNCTION__, __LINE__, message); std::exit(0);
#define WARNF(message) spdlog::warn("[{}]-[{}]-[{}]: {}", __FILE__, __FUNCTION__, __LINE__, message)
#define INFOF(message) spdlog::info("[{}]-[{}]-[{}]: {}", __FILE__, __FUNCTION__, __LINE__, message)
#define DEBF(message) spdlog::debug("[{}]-[{}]-[{}]: {}", __FILE__, __FUNCTION__, __LINE__, message)

#define ERROR_IF(statement, message, ...) if (statement) { \
    ERROR(message, __VA_ARGS__); \
}

#define WARN_IF(statement, message, ...) if (statement) { \
    WARN(message, __VA_ARGS__); \
}

#define ERROR_IFF(statement, message) if (statement) { \
    ERRORF(message); \
}

#define WARN_IFF(statement, message) if (statement) { \
    WARNF(message); \
}

} // moss
