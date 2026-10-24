/**
 * @file    meta/logs.hpp
 * @brief   Log macros (spdlog) and the fatal-error policy.
 *
 * M_ERROR / M_WARN / M_INFO / M_DEB     log with source location.
 * M_*_IF(cond, ...)                     log when cond is true.
 * M_FATAL(...)                          log an error, then abort. No exceptions in moss.
 * M_FATAL_IF(cond, ...)                 M_FATAL when cond is true.
 *
 * The format string must be a compile-time literal with {} placeholders. The
 * "F" variants take no arguments (the string is then used as is).
 */

#pragma once

#include <moss/meta/libs.hpp>

#define M_SA(condition, message) static_assert(condition, "[MOSS] " message)

#define M_LOG(level, message, ...) spdlog::log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, level, message, ##__VA_ARGS__)

#define M_ERROR(message, ...) M_LOG(spdlog::level::err, message, ##__VA_ARGS__)
#define M_WARN(message, ...)  M_LOG(spdlog::level::warn, message, ##__VA_ARGS__)
#define M_INFO(message, ...)  M_LOG(spdlog::level::info, message, ##__VA_ARGS__)
#define M_DEB(message, ...)   M_LOG(spdlog::level::debug, message, ##__VA_ARGS__)

#define M_ERRORF(message) M_ERROR(message)
#define M_WARNF(message)  M_WARN(message)
#define M_INFOF(message)  M_INFO(message)
#define M_DEBF(message)   M_DEB(message)

#define M_ERROR_IF(cond, message, ...) do { if (cond) M_ERROR(message, ##__VA_ARGS__); } while (0)
#define M_WARN_IF(cond, message, ...) do { if (cond) M_WARN(message, ##__VA_ARGS__); } while (0)
#define M_INFO_IF(cond, message, ...) do { if (cond) M_INFO(message, ##__VA_ARGS__); } while (0)
#define M_DEB_IF(cond, message, ...) do { if (cond) M_DEB(message, ##__VA_ARGS__); } while (0)

#define M_ERROR_IFF(cond, message) do { if (cond) M_ERRORF(message); } while (0)
#define M_WARN_IFF(cond, message) do { if (cond) M_WARNF(message); } while (0)
#define M_INFO_IFF(cond, message) do { if (cond) M_INFOF(message); } while (0)
#define M_DEB_IFF(cond, message) do { if (cond) M_DEBF(message); } while (0)

#define M_FATAL(message, ...)                   \
    do {                                        \
        M_ERROR(message, ##__VA_ARGS__);        \
        std::abort();                           \
    } while (0)

#define M_FATALF(message)                       \
    do {                                        \
        M_ERRORF(message);                      \
        std::abort();                           \
    } while (0)

#define M_FATAL_IF(cond, message, ...) do { if (cond) M_FATAL(message, ##__VA_ARGS__); } while (0)
#define M_FATAL_IFF(cond, message) do { if (cond) M_FATALF(message); } while (0)
