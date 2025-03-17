/*
core/app.hpp - parentless

Core application.

*/

#pragma once

#include <moss/core/scene.hpp>
#include <moss/render/render.hpp>
#include <moss/meta/defines.hpp>


namespace moss {

class Container {
public:
    virtual Container& init();
    virtual Container& build();

    void package(const std::string* id);

protected:
    entt::registry m_registry; // contains packages
};

namespace containers::main {

namespace ScenePackage {

class System : moss::System {
    void init();
    void tick();
};

} // containers::main

class Main : public Container {
public:
    Main& init() override { 
        auto c = m_registry.create();
        m_registry.emplace<main::ScenePackage::init>(c);

        return *this;
    };
    Main& build() override;
};

};


namespace app {

struct BuildCrate {
    const std::string& dataDirectory = "data";
};

} // app

enum SystemLifecycle {
    Init,
    Tick,
    Exit
};


class App {
public:
    inline App() { };
    inline ~App() { };

    App& init();
    App& create();
    App& create(glm::u32 count);

    template<typename T> App& attachComponent();
    template<typename T> App& attachComponent(entt::entity entity);
    template<typename T> App& attachComponent(std::initializer_list<entt::entity> entities);

    App& attachSystem(const std::function<const std::unique_ptr<sys::Crate>&>& sys);
    App& attachSystem(const std::function<const std::unique_ptr<sys::Crate>&>& sys, entt::entity entity);
    App& attachSystem(const std::function<const std::unique_ptr<sys::Crate>&>& sys, std::initializer_list<entt::entity> entities);

    /*template<typename T> App& attachPackage(const std::string& id);*/
    /*template<typename T> App& attachPackage(const entt::entity& entity, const std::string& id);*/
    /*template<typename T> App& attachPackage(const std::initializer_list<entt::entity>& entity, const std::string& id);*/

    /*template<typename T> App& attatchContainer();*/
    /*Container& container();*/

    App& attachScene(const std::string& id, bool currentScene);
    App& attachScene(std::initializer_list<std::pair<const std::string&, bool>> scenes);


    App& build(app::BuildCrate crate);
    App& run();
    void exit();

private:
    entt::registry m_registry;
    std::vector<entt::entity> m_latest;
};

} // moss
