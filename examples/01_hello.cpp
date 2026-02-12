#include <clixxi/app.hpp>
#include <iostream>

#define APP_NAME "example_hello"
#define APP_DESC "Simple app created by Clixxi."
#define APP_VERSION "1.0"

int main(int argc, char* argv[]) {
    Clixxi::App app(APP_NAME, APP_DESC, APP_VERSION);

    app.command("cmd1", "First command in this app.")
        .option("opt1", "First option for this command")
        .option("opt2", "Second option for this command")
        .run([](const Clixxi::Context& context) {
            std::cout << "opt1: " << context.get_option<std::string>("opt1") << "\n" << "opt2: " << context.get_option<bool>("opt2", true);
    });

    try{
    app.run(argc, argv);
    }
    catch(const Clixxi::Exception& e){
        Clixxi::error << e.what() << std::endl;
        return 1;
    }
    return 0;
}