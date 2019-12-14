#include "processor.h"
#include "linux_parser.h"
#include <vector>
#include <sstream>
#

// DONE: Return the aggregate CPU utilization
float Processor::Utilization() {
	std::vector<std::string> utils = LinuxParser::CpuUtilization();
	std::string utilstr = utils[0];
	float util;
	std::stringstream conv;
	conv << utilstr;
	conv >> util;
	return util;
}
