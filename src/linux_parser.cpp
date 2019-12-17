#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <utility>
#include <unordered_map>
#include "linux_parser.h"
//#include "/home/workspace/git/CppND-System-Monitor/include/linux_parser.h"
using std::stof;
using std::string;
using std::to_string;
using std::vector;
typedef std::unordered_map<std::string,int>::const_iterator unordered_iter;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// DONE: Read and return the system memory utilization
// /proc/meminfo
// 
float LinuxParser::MemoryUtilization() { 
	string line;
	string key;
	int value;
	std::ifstream filestream(kProcDirectory + kMeminfoFilename);
	std::unordered_map<std::string,int> mymap;
	if (filestream.is_open()) {
		while (std::getline(filestream, line)) {
			std::replace(line.begin(), line.end(), ':', ' ');
			std::istringstream linestream(line);
//HERE
			while (linestream >> key >> value) {
				mymap.insert(std::make_pair(key,value));
			}	
		}
	}
	unordered_iter iMemTotal     = mymap.find("MemTotal");
	unordered_iter iMemFree      = mymap.find("MemFree");
	unordered_iter iBuffers      = mymap.find("Buffers");
	unordered_iter iCached       = mymap.find("Cached");
	unordered_iter iShmem        = mymap.find("Shmem");
	unordered_iter iSReclaimable = mymap.find("SReclaimable");
	unordered_iter iSwapTotal    = mymap.find("SwapTotal");
	unordered_iter iSwapFree     = mymap.find("SwapFree");
	int MemTotal     = iMemTotal->second;
	int MemFree      = iMemFree->second;
	int Buffers      = iBuffers->second;
	int Cached       = iCached->second;
	int Shmem        = iShmem->second;
	int SReclaimable = iSReclaimable->second;
	int SwapTotal    = iSwapTotal->second;
	int SwapFree     = iSwapFree->second;

	int TotalUsedMemory       = MemTotal-MemFree;
	//std::cout << MemTotal << " " << MemFree << " " << TotalUsedMemory << std::endl;
	int CachedMemory          = Cached+SReclaimable;
	int NonCache_BufferMemory = TotalUsedMemory-(Buffers+CachedMemory);
	int Swap                  = SwapTotal-SwapFree;

	//std::cout << MemTotal << " " << MemFree << " " << TotalUsedMemory << std::endl;


	float utilization         = TotalUsedMemory*1.0/MemTotal;
	return utilization;
}


// DONE: Read and return the system uptime
//  /proc/uptime
long LinuxParser::UpTime() {
	string line;
	long uptime,idle_time;
	std::ifstream filestream(kProcDirectory + kUptimeFilename);
	if (filestream.is_open()) {
		while (std::getline(filestream, line)) {
			std::istringstream linestream(line);
			linestream >> uptime >> idle_time;
		}
	}
	return uptime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {





	return 0; 
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

float LinuxParser::ProcessUtilization(int pid) {
	string line;
	string token;
	vector<std::string> tokens;
	std::stringstream conv;
	std::string pid_str;
	conv << pid;
	conv >> pid_str;
	std::ifstream filestream(kProcDirectory + pid_str + kStatFilename);

	if (filestream.is_open()) {
		while (std::getline(filestream, line)) {
			std::istringstream linestream(line);
			while (linestream >> token) {
				tokens.push_back(token);
			}
		}
	}

   long utime;
   long stime;
   long cutime;
   long cstime;
   long starttime;
   conv << tokens[13];
   conv >> utime;
   conv << tokens[14];
   conv >> stime;
   conv << tokens[15];
   conv >> cutime;
   conv << tokens[16];
   conv >> cstime;
   conv << tokens[21];
   conv >> starttime;

   long system_uptime = LinuxParser::UpTime();


   //  Calculate the utilization
   //-----------------------------
   long total_time = utime + stime + cutime + cstime;
   long seconds    = system_uptime - (starttime/sysconf(_SC_CLK_TCK));
   float cpu_pct   = 100.0*((total_time/sysconf(_SC_CLK_TCK))/seconds);

/*
	std::cout << "system uptime " << system_uptime << std::endl;
	std::cout << "starttime " << starttime << std::endl;
	std::cout << "total_time " << total_time << std::endl;
	std::cout << "sysconf " << sysconf(_SC_CLK_TCK) << std::endl;
	std::cout << "seconds " << seconds << std::endl;
*/


   return cpu_pct;
}





// DONE: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
	string line;
	string header;
	string token;
	vector<long> tokens;
	vector<string> res;
	std::ifstream filestream(kProcDirectory + kStatFilename);
	if (filestream.is_open()) {
		while (std::getline(filestream, line)) {
			std::istringstream linestream(line);
			linestream >> header;
			if (header==string("cpu")) {
				while (linestream >> token) {
					std::stringstream conv;
					long item;
					conv << token;
					conv >> item;
					tokens.push_back(item);
				}
				std::cout << std::endl;
			}
		}
	}
	long user       = tokens[0];
	long nice       = tokens[1];
	long system     = tokens[2];
	long idle       = tokens[3];
	long iowait     = tokens[4];
	long irq        = tokens[5];
	long softirq    = tokens[6];
	long steal      = tokens[7];
	long guest      = tokens[8];
	long guest_nice = tokens[9];

	//  Calculate the utilization
	//-----------------------------
	long usertime  = user - guest;
	long nicetime  = nice - guest_nice;
	long idleall   = idle + iowait;
	long systemall = system + irq + softirq;
	long virtall   = guest + guest_nice;
	long total     = usertime + nicetime + systemall + idleall + steal + virtall;
	long Idle    = idle + iowait;
	long NonIdle = user + nice + system + irq + softirq + steal;
	long Total   = Idle+NonIdle;
	float cpu_util = 1.0*NonIdle/Total;
	std::stringstream conv;
	conv << cpu_util;
	std::string util = conv.str();
	util += string("%");
	res.push_back(util);
	//std::cout << util << std::endl;
	return res;

} 

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
	string line;
	string first;
	int second;
	std::ifstream filestream(kProcDirectory + kStatFilename);
	if (filestream.is_open()) {
		while (std::getline(filestream, line)) {
			std::istringstream linestream(line);
			linestream >> first >> second;
			if (first == string("processes")) {
				return second;
			}
		}
	}
}

// DONE: Read and return the number of running processes
// /proc/stat
int LinuxParser::RunningProcesses() {
	string line;
	string first;
	int second;
	std::ifstream filestream(kProcDirectory + kStatFilename);
	if (filestream.is_open()) {
		while (std::getline(filestream, line)) {
			std::istringstream linestream(line);
			linestream >> first >> second;
			if (first == string("procs_running")) {
				return second;
			}
		}
	}
}

// DONE: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
   string line;
   std::stringstream ss;
   std::string pid_str;
   ss << pid;
   ss >> pid_str;
   std::ifstream filestream(kProcDirectory + pid_str + kCmdlineFilename);
   if (filestream.is_open()) {
      while (std::getline(filestream, line)) {
			return line;
      }
   }
}


// DONE: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
	string line;
	string first;
	long second;
	std::stringstream ss;
	std::string pid_str;
	ss << pid;
	ss >> pid_str;
	std::ifstream filestream(kProcDirectory + pid_str + kStatusFilename);
	if (filestream.is_open()) {
		while (std::getline(filestream, line)) {
			std::replace(line.begin(), line.end(), ':', ' ');
			std::istringstream linestream(line);
			linestream >> first >> second;
			if (first==string("VmSize")) {
				float mem_in_M = second*1.00/1000;
				std::stringstream conv;
				conv << mem_in_M;
				return conv.str();
			}
		}
	}
}

// DONE: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
	string line;
	string first;
	int second;
	std::stringstream ss;
	std::string pid_str;
	ss << pid;
	ss >> pid_str;
	std::ifstream filestream(kProcDirectory + pid_str + kStatusFilename);
	if (filestream.is_open()) {
		while (std::getline(filestream, line)) {
			std::replace(line.begin(), line.end(), ':', ' ');
			std::istringstream linestream(line);
			linestream >> first >> second;
			if (first==string("Uid")) {
				std::stringstream conv;
				conv << second;
				return conv.str();
			}
		}
	}
}
				
// DONE: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
	string line;
	string username,x,uidstr;

	string uid = LinuxParser::Uid(pid);
	
	std::ifstream filestream(kPasswordPath);
	if (filestream.is_open()) {
		while (std::getline(filestream, line)) {
			std::replace(line.begin(), line.end(), ':', ' ');
			std::istringstream linestream(line);
			linestream >> username >> x >> uidstr;
			if (uidstr==uid) {
				return username;
			}
		}
	}
}

// DONE: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
	string token;
	string line;
	long uptime;
	std::stringstream ss;
	std::string pid_str;
	ss << pid;
	ss >> pid_str;
	std::ifstream filestream(kProcDirectory + pid_str + kStatFilename);
	if (filestream.is_open()) {
		while (std::getline(filestream, line)) {
			std::istringstream linestream(line);
			int count=0;
			while (count < 22) {
				linestream >> token;
				count++;
			}
		}
	}
	std::stringstream conv;
	conv << token;
	conv >> uptime;
	//return uptime;
	return uptime/sysconf(_SC_CLK_TCK);
}































