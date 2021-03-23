#ifndef __CLOCK_H_
#define __CLOCK_H_

#include <cstdlib>
#include <string>
#include <vector>
#include <map>

#include <ncpp/NotCurses.hh>
#include <ncpp/Visual.hh>
#include <ncpp/Plane.hh>

// Global Variables
// rows: 5, cols: 7
/*
const std::string zero = "000000\n00  00\n00  00\n00  00\n000000";
const std::string one = "1111  \n  11  \n  11  \n  11  \n111111";
const std::string two = "222222\n     2\n222222\n2     \n222222";
const std::string three = "333333\n    33\n333333\n    33\n333333";
const std::string four = "44  44\n44  44\n444444\n    44\n    44";
const std::string five = "555555\n55    \n555555\n    55\n555555";
const std::string six = "666666\n66    \n666666\n66  66\n666666";
const std::string seven = "777777\n    77\n    77\n    77\n    77";
const std::string eight = "888888\n88  88\n888888\n88  88\n888888";
const std::string nine = "999999\n99  99\n999999\n    99\n999999";
const std::string colon = "::\n\n\n\n::";
*/
const std::string zero = "██████\n██  ██\n██  ██\n██  ██\n██████";
const std::string one = "████  \n  ██  \n  ██  \n  ██  \n██████";
const std::string two = "██████\n     █\n██████\n█     \n██████";
const std::string three = "██████\n    ██\n██████\n    ██\n██████";
const std::string four = "██  ██\n██  ██\n██████\n    ██\n    ██";
const std::string five = "██████\n██    \n██████\n    ██\n██████";
const std::string six = "██████\n██    \n██████\n██  ██\n██████";
const std::string seven = "██████\n    ██\n    ██\n    ██\n    ██";
const std::string eight = "██████\n██  ██\n██████\n██  ██\n██████";
const std::string nine = "██████\n██  ██\n██████\n    ██\n██████";
// rows: 5, cols: 3
const std::string colon = "██\n\n\n\n██";

inline std::map<char, std::string> g_numMap = { { '0', zero},
                                                       { '1', one },
                                                       { '2', two },
                                                       { '3', three },
                                                       { '4', four },
                                                       { '5', five },
                                                       { '6', six },
                                                       { '7', seven },
                                                       { '8', eight },
                                                       { '9', nine },
                                                       { ':', colon}};

// Constants specific for the clock
inline extern const long NSEC = 200000000;

inline extern const int CLOCK_ROWS = 7;
inline extern const int CLOCK_COLS = 32;

inline extern const int COLON_ROWS = 5;
inline extern const int COLON_COLS = 3;

inline extern const int NUM_ROWS = 5;
inline extern const int NUM_COLS = 7;

// Planes and nc objects
extern ncpp::NotCurses* g_n;
extern ncpp::Plane* g_clockPlane;
extern ncpp::Visual* g_bgImage;

inline std::vector<ncpp::Plane*>* g_planes = new std::vector<ncpp::Plane*> ();

// Function prototypes
extern void
initPlanes ();

extern void
initClockPlane ();

extern void
initNumPlanes ();

extern void
initBg (char* file);

extern std::string
getTimeString ();

extern int
putStringsOnPlanes (const std::vector<std::string>& strings);

extern void
setFgPlanes ();

extern void
setFgPlanes (unsigned color);

extern void
processInput ();

extern unsigned
getColorInput ();


#endif // __CLOCK_H_
