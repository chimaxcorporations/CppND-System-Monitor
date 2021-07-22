#include <string>
#include "format.h"

string Format::addZero(long timeNum){
	string num = (timeNum<MAX_COUNT)? "0"+ to_string(timeNum): to_string(timeNum);
  return num;
}
// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long timeInt) {
  long hours = timeInt/SECONDS_PER_HOUR; // hour
  long minutes = (timeInt % SECONDS_PER_HOUR) /MINUTES_PER_HOUR; // minute
  long seconds = timeInt % MINUTES_PER_HOUR;// seconds
  return addZero(hours)+":"+ addZero(minutes) +":"+ addZero(seconds);
  }