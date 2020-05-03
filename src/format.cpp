#include <string>
#include <sstream>
#include <ios>
#include <iomanip>

#include "format.h"

using std::string;

// TODO DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
    int hh = 0;
    int mm = 0;
    int ss = 0;
    div_t div_res;

    div_res = div(seconds, 3600);
    hh = div_res.quot;

    div_res = std::div(div_res.rem, 60);
    mm = div_res.quot;
    ss = div_res.rem;

    std::stringstream res;
    res << std::setfill('0')
        << std::setw(2) << std::right << hh << ":"
        << std::setw(2) << std::right << mm << ":"
        << std::setw(2) << std::right << ss;

    return res.str();
}