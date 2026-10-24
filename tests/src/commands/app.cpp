#include <doctest/doctest.h>
#include <moss/moss.hpp>
#include <tst/simple.hpp>

#include <string>
#include <vector>

using namespace moss;

namespace {

using Log = std::vector<std::string>;

Log g_log;
void note(const std::string& line) { g_log.push_back(line); }

struct Counter { int ticks = 0; };

// Records every phase of itself
template<int N>
class Probe : public System {
public:
    void build(const Key<key::WRITE>&) override { note("build" + std::to_string(N)); }
    void tick(const Key<key::READ>&) override   { note("tick" + std::to_string(N)); }
    void exit(const Key<key::WRITE>&) override  { note("exit" + std::to_string(N)); }
};

// Asks the App to stop during its F-th tick
template<int F>
class StopAt : public System {
public:
    void tick(const Key<key::READ>& key) override {
        if (++m_frames == F) cmd::Quit::init(key).quit();
    }

private:
    int m_frames = 0;
};

class OrderContext : public Context {
public:
    void init(Mirror& mirror) override { mirror.connect<Probe<1>, Probe<2>, Probe<3>, StopAt<1>>(); }
};

class FirstContext : public Context {
public:
    void init(Mirror& mirror) override { mirror.connect<Probe<1>, Probe<2>, StopAt<1>>(); }
};

class SecondContext : public Context {
public:
    void init(Mirror& mirror) override { mirror.connect<Probe<3>, Probe<4>>(); }
};

class QuitMidFrameContext : public Context {
public:
    void init(Mirror& mirror) override { mirror.connect<Probe<1>, StopAt<3>, Probe<2>>(); }
};

// Owns a resource for the whole run, through the key-based commands
class ResourceSystem : public System {
public:
    void build(const Key<key::WRITE>& key) override {
        cmd::Resource<Counter>::init(key).emplace();
    }

    void tick(const Key<key::READ>& key) override {
        Counter& counter = cmd::Resource<Counter>::init(key).get();
        if (++counter.ticks == 4) cmd::Quit::init(key).quit();
    }

    void exit(const Key<key::WRITE>& key) override {
        auto res = cmd::Resource<Counter>::init(key);
        note("ticks=" + std::to_string(res.get().ticks));
        res.erase();
        note(res.contains() ? "still there" : "erased");
    }
};

class ResourceContext : public Context {
public:
    void init(Mirror& mirror) override { mirror.connect<ResourceSystem>(); }
};

// What the entities of a Context look like to its systems
struct Seen {
    usize entities = 0;
    int posSum = 0;
    usize flagged = 0;
    int replacedX = -1;
    bool unorderedOk = false;
};
Seen g_seen;

class Inspect : public System {
public:
    void tick(const Key<key::READ>& key, const DynamicView& entities) override {
        g_seen.entities = entities.size();

        auto rows = cmd::Query<With<tst::Pos>>::init(key).atlas(entities);
        for (auto& [pos] : rows) g_seen.posSum += pos.x;

        g_seen.flagged = cmd::Query<With<tst::Pos>, cmd::View<Include<tst::Flag>>>
            ::init(key).entities(entities).size();

        cmd::Quit::init(key).quit();
    }
};

class EntityContext : public Context {
public:
    void init(Mirror& mirror) override {
        mirror
            .connect<Inspect>()
            .create(2).attach<tst::Pos>({ .x = 5 })            // two entities, designated init
            .create().attach<tst::Pos, tst::Flag>()           // two types, defaults
            .create().attach<tst::Pos>(1, 1)                  // positional
                     .attach<tst::Pos>({ .x = 2 });           // replaces the line above
    }
};

}

TEST_CASE("App: lifecycle order") {
    g_log.clear();

    GIVEN("One context") {
        App app;
        app.mount<OrderContext>().build().run().exit();

        // StopAt<1> quits in the first frame, after the probes before it ticked
        Log expected{
            "build1", "build2", "build3",
            "tick1", "tick2", "tick3",
            "exit3", "exit2", "exit1"
        };
        CHECK(g_log == expected);
    }

    GIVEN("Two contexts") {
        App app;
        app.mount<FirstContext>().mount<SecondContext>().build().run().exit();

        // Build and tick in mount order, exit in reverse (contexts and systems)
        Log expected{
            "build1", "build2", "build3", "build4",
            "tick1", "tick2", "tick3", "tick4",
            "exit4", "exit3", "exit2", "exit1"
        };
        CHECK(g_log == expected);
    }
}

TEST_CASE("App: quit finishes the frame in progress") {
    g_log.clear();

    App app;
    app.mount<QuitMidFrameContext>().build().run().exit();

    // StopAt<3> quits on frame 3; Probe<2> after it still ticks that frame
    int ticks1 = 0, ticks2 = 0;
    for (const std::string& line : g_log) {
        if (line == "tick1") ticks1++;
        if (line == "tick2") ticks2++;
    }

    CHECK(ticks1 == 3);
    CHECK(ticks2 == 3);
}

TEST_CASE("App: resources through the key") {
    g_log.clear();

    App app;
    app.mount<ResourceContext>().build().run().exit();

    Log expected{ "ticks=4", "erased" };
    CHECK(g_log == expected);
}

TEST_CASE("App: a Context's entities reach its systems") {
    g_seen = Seen{};

    App app;
    app.mount<EntityContext>().build().run().exit();

    CHECK(g_seen.entities == 4);
    CHECK(g_seen.posSum == 5 + 5 + 0 + 2);   // the last entity's Pos was replaced by x = 2
    CHECK(g_seen.flagged == 1);
}
