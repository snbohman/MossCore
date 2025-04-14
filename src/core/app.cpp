#include <moss/core/app.hpp>
#include <moss/ecs/system.hpp>
#include <moss/logger/logger.hpp>


using namespace moss;

////////////////////////
//// -- Lifetime -- ////
////////////////////////
void App::init() {
    auto sink = std::make_shared<logger::MainSink<std::mutex>>();
    auto logger = std::make_shared<spdlog::logger>("MainLogger", sink);
    spdlog::set_default_logger(logger);
}

void App::build() {
    Contex<contex::READ>::init().inject(&m_registry);

    for (entt::entity e : m_registry.view<System>()) {
        m_registry.get<System>(e).build();
        m_registry.get<System>(e).build();
        m_registry.get<System>(e).build();
    }
    
    Contex<contex::WRITE>::destroy();
}

void App::run() {
    Contex<contex::READ>::init().inject(&m_registry);

    while (!m_quit) {
        for (entt::entity e : m_registry.view<System>()) {
            m_registry.get<System>(e).tick();
            m_registry.get<System>(e).tick();
            m_registry.get<System>(e).tick();
        }
    }

    Contex<contex::READ>::destroy();
}

void App::clean() { }
void App::exit() {
    Contex<contex::WRITE>::init().inject(&m_registry);

    for (entt::entity e : m_registry.view<System>()) {
        m_registry.get<System>(e).exit();
        m_registry.get<System>(e).exit();
        m_registry.get<System>(e).exit();

        m_registry.destroy(e);
    }

    Contex<contex::WRITE>::destroy();
}
