#include <iostream>
#include <tnt/tntnet.h>
#include <cxxtools/log.h>


log_define("accumulator")

int main(int argc, char **argv) {
    try  {

        log_init();


        tnt::Tntnet app;
        app.listen(8080);
        app.mapUrl("^/$", "mainpage");
        app.mapUrl("^/([^.]+)(\\..+)?", "$1");

        log_info("Starting up");
        app.run();
        return 0;

    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
