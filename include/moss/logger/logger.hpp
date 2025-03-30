#pragma once

#include <moss/meta/libs.hpp>
#include <spdlog/sinks/base_sink.h>

namespace moss::logger {

template<typename Mutex>
class MainSink : public spdlog::sinks::base_sink<Mutex> {
protected:
    void sink_it_(const spdlog::details::log_msg& msg) override {
        std::string formatted = fmt::format(
            "[MOSS] - [{}]-[{}]-[{}]: {}",
            msg.source.filename,    // File
            msg.source.funcname,    // Function
            msg.source.line,        // Line
            std::string(msg.payload.data(), msg.payload.size())
        );

        spdlog::details::log_msg formatted_msg = msg;
        formatted_msg.payload = formatted;
        spdlog::sinks::base_sink<Mutex>::formatter_->format(formatted_msg);
        spdlog::sinks::base_sink<Mutex>::sink_it_(formatted_msg);

        if (msg.level >= spdlog::level::err) {
            throw std::runtime_error(formatted);
        }
    }

    void flush_() override { }
};

} // moss::logger
