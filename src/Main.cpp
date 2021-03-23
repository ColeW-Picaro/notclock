#include <cstdlib>
#include <string>
#include <vector>

#include <ncpp/NotCurses.hh>

#include "Clock.hpp"

extern ncpp::NotCurses* g_n;

int
main(int argc, char *argv[]) {
    setlocale(LC_ALL, "");
    g_n = new ncpp::NotCurses (stdout);
    // TODO: Resize handler
    initPlanes();
    if (argc == 2) {
        initBg(argv[1]);
    }

    // Main render loop
    while (g_n) {
        std::string stdTimeString = getTimeString ();

        // Write each string to its plane
        std::vector<std::string> strings =
            { g_numMap[stdTimeString[11]],
              g_numMap[stdTimeString[12]],
              g_numMap[stdTimeString[14]],
              g_numMap[stdTimeString[15]],
              g_numMap[stdTimeString[13]], };

        putStringsOnPlanes(strings);
        g_n->render();
        processInput();
    }

    return EXIT_SUCCESS;
}
