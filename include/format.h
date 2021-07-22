#ifndef FORMAT_H
#define FORMAT_H
#include <string>
#define SECONDS_PER_HOUR 3600
#define MINUTES_PER_HOUR 60
#define MAX_COUNT 10
using std::string;
using std::to_string;
namespace Format {
string ElapsedTime(long times);  // TODO: See src/format.cpp
string addZero(long timeNum);
};                                    // namespace Format
#endif