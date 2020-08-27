#include <string>

#include "format.h"

using std::string;
using std::to_string;

// Done: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) { 
    int hours;
    int minutes;
    int secs;
    hours = seconds / 3600;
    seconds -= hours * 3600;
    minutes = seconds / 60;
    seconds -= minutes * 60;
  	string hours_string = (hours < 10 ? "0" : "") + to_string(hours);
  	string minutes_string = (minutes < 10 ? "0" : "") + to_string(minutes);
  	string seconds_string = (seconds < 10 ? "0" : "") + to_string(seconds);
    return hours_string + ":" + minutes_string + ":" + seconds_string;
}