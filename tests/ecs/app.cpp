
TEST_CASE("Basic App") {
    App app = moss::App::init();

    Contex contex;
    app.loadContex(contex);

    Create<10> c;
    DynamicView v = c.apply(contex);
}
