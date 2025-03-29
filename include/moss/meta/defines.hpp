/*
meta/defines.hpp

Defines log macros, utelizing spdlog. Defines
long types, such as AttatchmentRegistry.

*/


#pragma once

#include <spdlog/spdlog.h>
#include <fmt/format.h>
#include <entt/entt.hpp>

namespace moss {

extern int infoStep1;
extern int infoStep2;
extern int infoStep3;

#define M_INFO_INIT(message, infoStep, ...) \
    if (infoStep == 1) { infoStep1++; infoStep2 = 0; infoStep3 = 0; } \
    else if (infoStep == 2) { infoStep2++;  infoStep3 = 0; } \
    else if (infoStep == 3) { infoStep3++; } \
    spdlog::info("[{}].[{}].[{}]\t >> {}", infoStep1, infoStep2, infoStep3, fmt::format(message, __VA_ARGS__));

#define M_INFO_INITF(message, infoStep) \
    if (infoStep == 1) { infoStep1++; infoStep2 = 0; infoStep3 = 0; } \
    else if (infoStep == 2) { infoStep2++;  infoStep3 = 0; } \
    else if (infoStep == 3) { infoStep3++; } \
    spdlog::info("[{}].[{}].[{}]\t >> {}", infoStep1, infoStep2, infoStep3, message);

#define M_ERROR(message, ...) spdlog::error("[{}]-[{}]-[{}]: {}", __FILE__, __FUNCTION__, __LINE__, fmt::format(message, __VA_ARGS__)); std::exit(0);
#define M_WARN(message, ...) spdlog::warn("[{}]-[{}]-[{}]: {}", __FILE__, __FUNCTION__, __LINE__, fmt::format(message, __VA_ARGS__))
#define M_DEB(message, ...) spdlog::debug("[{}]-[{}]-[{}]: {}", __FILE__, __FUNCTION__, __LINE__, fmt::format(message, __VA_ARGS__))
#define M_INFO(message, ...) spdlog::info("[{}]-[{}]-[{}]: {}", __FILE__, __FUNCTION__, __LINE__, fmt::format(message, __VA_ARGS__))

#define M_ERRORF(message) spdlog::error("[{}]-[{}]-[{}]: {}", __FILE__, __FUNCTION__, __LINE__, message); std::exit(0);
#define M_WARNF(message) spdlog::warn("[{}]-[{}]-[{}]: {}", __FILE__, __FUNCTION__, __LINE__, message)
#define M_INFOF(message) spdlog::info("[{}]-[{}]-[{}]: {}", __FILE__, __FUNCTION__, __LINE__, message)
#define M_DEBF(message) spdlog::debug("[{}]-[{}]-[{}]: {}", __FILE__, __FUNCTION__, __LINE__, message)

#define M_ERROR_IF(statement, message, ...) if (statement) { \
    M_ERROR(message, __VA_ARGS__); \
}

#define M_WARN_IF(statement, message, ...) if (statement) { \
    M_WARN(message, __VA_ARGS__); \
}

#define M_ERROR_IFF(statement, message) if (statement) { \
    M_ERRORF(message); \
}

#define M_WARN_IFF(statement, message) if (statement) { \
    M_WARNF(message); \
}

} // moss
