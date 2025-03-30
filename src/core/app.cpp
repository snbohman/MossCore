#include <moss/core/app.hpp>
#include <moss/logger/logger.hpp>


void moss::App::init() {
    auto sink = std::make_shared<logger::MainSink<std::mutex>>();
    auto logger = std::make_shared<spdlog::logger>("MainLogger", sink);
    spdlog::set_default_logger(logger);
}
