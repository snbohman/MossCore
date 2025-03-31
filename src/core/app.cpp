#include <moss/core/app.hpp>
#include <moss/ecs/system.hpp>
#include <moss/logger/logger.hpp>


using namespace moss;

////////////////////
//// -- User -- ////
////////////////////
void App::apply(Contex<contex::READ>& contex) {
    contex.registry = &m_registry;
}

void App::apply(Fluent<fluent::WRITE>& fluent) {
    fluent.apply(&m_registry);
}


////////////////////////
//// -- Lifetime -- ////
////////////////////////
void moss::App::init() {
    auto sink = std::make_shared<logger::MainSink<std::mutex>>();
    auto logger = std::make_shared<spdlog::logger>("MainLogger", sink);
    spdlog::set_default_logger(logger);
}

void moss::App::build() {
    Fluent<fluent::WRITE>::init().apply(&m_registry);
    Contex<contex::WRITE> contex;
    contex.registry = &m_registry;

    for (entt::entity e : m_registry.view<System>()) {
        m_registry.get<System>(e).build();
        m_registry.get<System>(e).build(contex);
        m_registry.get<System>(e).build(contex, e);
    }
    
    Fluent<fluent::WRITE>::destroy();
}

void moss::App::run() {
    Fluent<fluent::WRITE>::init().apply(&m_registry);
    Contex<contex::READ> contex;
    contex.registry = &m_registry;

    while (!m_quit) {
        for (entt::entity e : m_registry.view<System>()) {
            m_registry.get<System>(e).tick();
            m_registry.get<System>(e).tick(contex);
            m_registry.get<System>(e).tick(contex, e);
        }
    }

    Fluent<fluent::WRITE>::destroy();
}

void moss::App::clean() { }
void moss::App::exit() {
    Fluent<fluent::WRITE>::init().apply(&m_registry);
    Contex<contex::WRITE> contex;
    contex.registry = &m_registry;

    for (entt::entity e : m_registry.view<System>()) {
        m_registry.get<System>(e).exit();
        m_registry.get<System>(e).exit(contex);
        m_registry.get<System>(e).exit(contex, e);

        m_registry.destroy(e);
    }

    Fluent<fluent::WRITE>::destroy();
}
