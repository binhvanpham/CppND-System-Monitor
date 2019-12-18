#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;


// DONE: Return the system's CPU
Processor& System::Cpu() {
	return cpu_;
}

// DONE: Return a container composed of the system's processes
vector<Process>& System::Processes() {
	vector<int> pids = LinuxParser::Pids();
	vector<Process> processes;
	for (auto pid : pids) {
		Process thread(pid);
		processes.push_back(thread);
	}

	sort(processes.begin(),processes.end(),[](const Process& a, const Process& b) {
		return (b.CpuUtilization() < a.CpuUtilization());
	});

	processes_ = processes;
	return processes_;	
}

// DONE: Return the system's kernel identifier (string)
std::string System::Kernel() {
	return LinuxParser::Kernel();
}

// DONE: Return the system's memory utilization
float System::MemoryUtilization() {
	return LinuxParser::MemoryUtilization();
}

// DONE: Return the operating system name
std::string System::OperatingSystem() {
	return LinuxParser::OperatingSystem();
}

// DONE: Return the number of processes actively running on the system
int System::RunningProcesses() {
	return LinuxParser::RunningProcesses();
}

// DONE: Return the total number of processes on the system
int System::TotalProcesses() {
	return LinuxParser::TotalProcesses();
}

// DONE: Return the number of seconds since the system started running
long int System::UpTime() {
	return LinuxParser::UpTime();
}
