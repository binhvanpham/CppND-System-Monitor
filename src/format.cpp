#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>

#include "format.h"

using std::string;

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
	int hours = seconds/3600;
	seconds   = seconds%3600;
	int mins  = seconds/60;
	seconds   = seconds%60;

	//char timestamp[10];
	//sprintf(timestamp,"%02d:%02d:%02ld",hours,mins,seconds);
	//return timestamp;

	std::stringstream ss;
	ss << std::setfill('0') << std::setw(2) << hours << ':';
	ss << std::setfill('0') << std::setw(2) << mins << ':';
	ss <<	std::setfill('0') << std::setw(2) << seconds;
	
	return ss.str();
}

