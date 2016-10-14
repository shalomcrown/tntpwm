#include <iostream>
#include <tnt/tntnet.h>

int main(int argc, char **argv) {
    try  {
        tnt::Tntnet app;
        app.listen(8000);
        app.mapUrl("^/$", "mainpage");
        app.mapUrl("^/([^.]+)(\\..+)?", "$1");
        app.run();
        return 0;

    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
