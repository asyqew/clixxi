#include <clixxi/app.hpp>
#include <iostream>

#define APP_NAME "example_hello"
#define APP_DESC "Simple app created by Clixxi."
#define APP_VERSION "1.0"

int main(int argc, char* argv[]) {
    Clixxi::App app(APP_NAME, APP_DESC, APP_VERSION);

    app.command("sum")
        .option("a")
        .option("b")
        .run([](const Clixxi::Context& ctx) {
            int a = ctx.get_option<int>("a");
            int b = ctx.get_option<int>("b");
            std::cout << "Result: " << a + b << std::endl;
        });

    try {
        app.run(argc, argv);
    } catch (const Clixxi::Exception& e) {
        Clixxi::error << e.what() << std::endl;
        return 1;
    }
    return 0;
}