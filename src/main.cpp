#include "ncurses_display.h"
#include "system.h"
//#include "linux_parser.cpp"
int main() {
	System system;
	NCursesDisplay::Display(system);

/*
	std::cout << LinuxParser::Kernel() << std::endl;
	std::cout << LinuxParser::MemoryUtilization() << std::endl;
	std::cout << LinuxParser::UpTime() << std::endl;
	std::cout << LinuxParser::TotalProcesses() << std::endl;
	std::cout << LinuxParser::RunningProcesses() << std::endl;
	std::cout << "-->" << LinuxParser::Command(22) << "<--" << std::endl;
	std::cout << "-->" << LinuxParser::Ram(22) << "<--" << std::endl;
	std::cout << "-->" << LinuxParser::Uid(22) << "<--" << std::endl;
	std::cout << "-->" << LinuxParser::User(22) << "<--" << std::endl;
	std::cout << "UPTIME-->" << LinuxParser::UpTime(22) << "<--" << std::endl;
	LinuxParser::CpuUtilization();
	vector<int> pids = LinuxParser::Pids();
	for (auto pid:pids) {
		std::cout << pid << " ";
	}
	std::cout << std::endl;
*/
}
