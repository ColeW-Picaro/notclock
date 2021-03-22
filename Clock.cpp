/*
** Filename: main.cpp
** Author: Cole Vohs
** Description: A simple clock program using notcurses
 */
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <chrono>
#include <ctime>
#include <vector>
#include <unistd.h>
#include <map>

#include <ncpp/NotCurses.hh>
#include <ncpp/Visual.hh>
#include <ncpp/Plane.hh>
#include <ncpp/Reader.hh>

#include "Clock.hpp"

int
main(int argc, char *argv[]) {
    setlocale(LC_ALL, "");
    n = new ncpp::NotCurses (stdout);
    // TODO: Resize handler
    initPlanes();
    if (argc == 2) {

        initBg(argv[1]);
    }

    if (n) {
        // Main render loop
        while (n) {
            std::string stdTimeString = getTimeString ();

            // Write each string to its plane
            std::vector<std::string> strings =
                { numMap[stdTimeString[11]],
                  numMap[stdTimeString[12]],
                  numMap[stdTimeString[14]],
                  numMap[stdTimeString[15]],
                  numMap[stdTimeString[13]], };

            putStringsOnPlanes(strings);
            n->render();
            processInput();
        }
    }

    return EXIT_SUCCESS;
}

// Initialize global nc variables
// n - the nc instance
// clockPlane - the plane holding the clock
// colonPlane - plane for the colon
// planes - std::vector holding the clock planes
void
initPlanes() {
    initClockPlane ();
    initNumPlanes ();
}

void
initClockPlane () {
    ncpp::Plane* stdplane = n->get_stdplane();
    int yMid = stdplane->get_dim_y() / 2;
    int xMid = stdplane->get_dim_x() / 2;
    ncplane_options clockOpts =
        {.y = yMid - clockRows / 2,
         .x = xMid - (clockCols / 2),
         .rows = clockRows,
         .cols = clockCols };
    clockPlane = new ncpp::Plane (stdplane, clockOpts);
    clockPlane->set_bg_alpha (CELL_ALPHA_OPAQUE);
    clockPlane->set_fg_rgb (0x000000);
}

// Inits the planes on the clock plane
// Adds those planes to the planes vector in clock order with the colon last
void
initNumPlanes () {
    // Colon Plane
    int colonX = clockPlane->get_dim_x() / 2 - 1;
    ncplane_options colonOpts =
        { .y = 1,
          .x = colonX,
          .rows = colonRows,
          .cols = colonCols };
    ncpp::Plane* colonPlane = new ncpp::Plane (clockPlane, colonOpts);

    // Number planes
    ncplane_options pOpts = { .y = 1, .x = colonX - (numCols * 2), .rows = numRows, .cols = numCols };
    for (int i = 0; i < 4; ++i) {
        planes->emplace_back (new ncpp::Plane (clockPlane, pOpts));
        (*planes)[i]->set_bg_alpha (CELL_ALPHA_OPAQUE);
        (*planes)[i]->set_fg_alpha (CELL_ALPHA_OPAQUE);
        if (i == 1) {
            pOpts.x += numCols + colonCols;
        } else {
            pOpts.x += numCols;
        }
    }
    planes->push_back (colonPlane);
}

void
initBg (char* file) {
    bgImage = new ncpp::Visual (file);
    ncpp::Plane* stdplane = n->get_stdplane ();
    struct ncvisual_options ncv_opts =
        { .n = stdplane->to_ncplane(),
          .scaling = NCSCALE_STRETCH,
          .begy = 0,
          .begx = 0,
          .blitter = NCBLIT_1x1,
          .flags = NCVISUAL_OPTION_BLEND | NCVISUAL_OPTION_NODEGRADE, };
    bgImage->render (&ncv_opts);
    //ncv_opts.n = clockPlane->to_ncplane ();
    //bgImage->render (&ncv_opts);
}

// Returns current time as a std::string
std::string getTimeString() {
    // Get the time as a string
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    char* timeString = std::ctime(&time);
    std::string stdTimeString (timeString);
    return stdTimeString;
}

// Call ncplane_putstr_yx on each of the planes and strings
// planes and strings are the same size
int putStringsOnPlanes (const std::vector<std::string>& strings) {
    if (planes->size () != strings.size ()) return 0;
    for (int i = 0; auto p : *planes) {
        p->putstr (0, 0, strings[i].c_str());
        ++i;
    }
    return 1;
}

void
setFgPlanes (unsigned color) {
    for (auto p : *planes) {
        p->set_fg_rgb(color);
    }
}

void
processInput () {
    ncinput in;
    timespec time = { .tv_nsec = NSEC };
    n->getc(&time, NULL, &in);
    if (in.id == 'c') {
        setFgPlanes(getColorInput ());
    }
}

// Make a plane
// Get input from the plane
// Turn the input string into unsigned hex value
unsigned
getColorInput () {
    ncinput in;
    ncplane_options opts = { .y = 0, .x = 0, .rows = 1, .cols = 20};
    ncpp::Plane* inputPlane = new ncpp::Plane (n->get_stdplane(), opts);
    inputPlane->putstr ("hex color: #");
    n->render();
    std::string color;
    while (n->getc (true, &in)) {
        if((in.ctrl && in.id == 'D') || in.id == NCKEY_ENTER){
            break;
        }
        inputPlane->putc (in.id);
        color.push_back(in.id);
        n->render ();
    }
    std::istringstream converter (color);
    unsigned value;
    converter >> std::hex >> value;
    delete inputPlane;
    return value;
}
