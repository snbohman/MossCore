#include <moss/core/app.hpp>
#include <moss/core/key.hpp>
#include <moss/ecs/system.hpp>
#include <moss/logger/logger.hpp>


using namespace moss;


App& App::instance() {
    static App app;
    return app;
}

App& App::init() {
    auto sink = std::make_shared<logger::MainSink<std::mutex>>();
    auto logger = std::make_shared<spdlog::logger>("MainLogger", sink);
    spdlog::set_default_logger(logger);

    return *this;
}

App& App::build() {
    for (Contex& ctx : m_contexts) {
        ctx.build();
    }

    return *this;
}

App& App::run() {
    while (!m_quit) {
        for (Contex& ctx : m_contexts) {
            ctx.tick();
        }
    }

    return *this;
}

App& App::exit() {
        for (Contex& ctx : m_contexts) {
            ctx.exit();
        }
}
