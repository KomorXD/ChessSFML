#include "App.hpp"

int main(int argc, char** argv) {
    if (argc > 1) {
        App app(argv[1]);

        app.Run();

        return 0;
    }

    App app;

    app.Run();

    return 0;
}