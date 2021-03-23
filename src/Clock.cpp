/*
** Filename: Clock.cpp
** Description: Implementation file for Clock.hpp
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
#include <ncpp/Cell.hh>

#include "Clock.hpp"

ncpp::NotCurses* g_n;
ncpp::Plane* g_clockPlane;
ncpp::Visual* g_bgImage;

std::string* bgFile;

struct ncvisual_options ncv_opts;

// Initialize global nc variables
// n - the nc instance
// clockPlane - the plane holding the clock
// colonPlane - plane for the colon
// g_planes - std::vector holding the clock planes
void
initPlanes() {
    ncv_opts =
        { .n = g_n->get_stdplane ()->to_ncplane (),
          .scaling = NCSCALE_STRETCH,
          .begy = 0,
          .begx = 0,
          .blitter = NCBLIT_2x1,
          .flags = NCVISUAL_OPTION_BLEND | NCVISUAL_OPTION_NODEGRADE, };
    initClockPlane ();
    initNumPlanes ();
}

void
initClockPlane () {
    ncpp::Plane* stdplane = g_n->get_stdplane();
    int yMid = stdplane->get_dim_y() / 2;
    int xMid = stdplane->get_dim_x() / 2;
    ncplane_options clockOpts =
        {.y = yMid - CLOCK_ROWS / 2,
         .x = xMid - (CLOCK_COLS / 2),
         .rows = CLOCK_ROWS,
         .cols = CLOCK_COLS };
    g_clockPlane = new ncpp::Plane (stdplane, clockOpts);
    g_clockPlane->set_bg_alpha (CELL_ALPHA_OPAQUE);
    g_clockPlane->set_fg_rgb (0x000000);
}

// Inits the planes on the clock plane
// Adds those planes to the planes vector in clock order with the colon last
void
initNumPlanes () {
    // Colon Plane
    int colonX = g_clockPlane->get_dim_x() / 2 - 1;
    ncplane_options colonOpts =
        { .y = 1,
          .x = colonX,
          .rows = COLON_ROWS,
          .cols = COLON_COLS };
    ncpp::Plane* colonPlane = new ncpp::Plane (g_clockPlane, colonOpts);

    // Number planes
    ncplane_options pOpts = { .y = 1, .x = colonX - (NUM_COLS * 2), .rows = NUM_ROWS, .cols = NUM_COLS };
    for (int i = 0; i < 4; ++i) {
        g_planes->emplace_back (new ncpp::Plane (g_clockPlane, pOpts));
        (*g_planes)[i]->set_bg_alpha (CELL_ALPHA_OPAQUE);
        (*g_planes)[i]->set_fg_alpha (CELL_ALPHA_OPAQUE);
        if (i == 1) {
            pOpts.x += NUM_COLS + COLON_COLS;
        } else {
            pOpts.x += NUM_COLS;
        }
    }
    g_planes->push_back (colonPlane);
}

void
initBg () {
    g_bgImage = new ncpp::Visual (bgFile->c_str());
    g_bgImage->render (&ncv_opts);
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
int
putStringsOnPlanes (const std::vector<std::string>& strings) {
    if (g_planes->size () != strings.size ()) return 0;
    for (int i = 0; auto p : *g_planes) {
        p->putstr (0, 0, strings[i].c_str());
        ++i;
    }
    return 1;
}

void
setFgPlanes (unsigned color) {
    for (auto p : *g_planes) {
        p->set_fg_rgb(color);
    }
}

void
processInput () {
    ncinput in;
    timespec time = { .tv_nsec = NSEC };
    g_n->getc(&time, NULL, &in);
    if (in.id == 'c') {
        setFgPlanes(getColorInput ());
    } else if (in.id == 'e') {
        g_n->get_stdplane()->erase();
    } else if (in.id == 'b') {
        if (g_bgImage != nullptr) {
            g_bgImage->render (&ncv_opts);
        }
    }
}


// Make a plane
// Get input from the plane
// Turn the input string into unsigned hex value
unsigned
getColorInput () {
    ncinput in;
    ncplane_options opts = { .y = 0, .x = 0, .rows = 1, .cols = 20};
    ncpp::Plane* inputPlane = new ncpp::Plane (g_n->get_stdplane(), opts);
    inputPlane->putstr ("hex color: #");
    g_n->render();
    std::string color;
    while (g_n->getc (true, &in)) {
        if((in.ctrl && in.id == 'D') || in.id == NCKEY_ENTER){
            break;
        }
        inputPlane->putc (in.id);
        color.push_back(in.id);
        g_n->render ();
    }
    std::istringstream converter (color);
    unsigned value;
    converter >> std::hex >> value;
    delete inputPlane;
    return value;
}
