#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include "linux_parser.h"
#define SCALE_DOWN 1000
#define MAX_JIF_COUNT 4
#define MAX_MEM_COUNT 5

using std::stof;
using std::string;
using std::to_string;
using std::vector;

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
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
    std::string memName, size, unit;
  std::string line;
  std::vector<float> memsize;
  std::ifstream stream(kProcDirectory+kMeminfoFilename);
   if (stream.is_open()) {
       for(auto i = 0; i<MAX_MEM_COUNT; ++i){
        std::getline(stream, line);
        std::istringstream linestream(line);
        linestream >> memName >> size >> unit;
        memsize.push_back(stof(size));
    }
   }
     stream.close();
  float memUtilization = (memsize.at(0)-memsize.at(1) - memsize.at(3)-memsize.at(4))/memsize.at(0);
  return memUtilization; }


// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  string line, currentTime;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> currentTime;
  }
  long upTime = stol(currentTime);
  return upTime; }

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
std::string memName, size, line;
long totalJif;
std::vector<std::string> totalItem;
std::ifstream stream(kProcDirectory + kStatFilename);
   if (stream.is_open())
    {
        std::getline(stream, line);
        std::istringstream linestream(line);
        while (linestream >> memName >> size)
        {
          totalItem.push_back(memName);
          totalItem.push_back(size);
        }
        totalItem.erase(totalItem.begin());
        for(auto &s : totalItem)
        {
          totalJif+=stol(s);
        }
        
   }
     stream.close();
return totalJif; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
  std::string memName, line; 
  long int jif_val{0}, active_jiffies{0};
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
     for (int i = 0; i < 13; ++i) {
      linestream >> memName;
    }
    for (int i = 0; i < MAX_JIF_COUNT; ++i)
    {
      linestream >> jif_val;
      active_jiffies += jif_val;
    }
    return active_jiffies;
  }
  return active_jiffies; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() 
{
    std::vector<string> CPU_jiffies = CpuUtilization();
  long activeJiffies = stol(CPU_jiffies[CPUStates::kUser_]) + stol(CPU_jiffies[CPUStates::kNice_]) +stol(CPU_jiffies[CPUStates::kSystem_]) + 		 	stol(CPU_jiffies[CPUStates::kIRQ_]) + stol(CPU_jiffies[CPUStates::kSoftIRQ_]) +stol(CPU_jiffies[CPUStates::kSteal_]); 
   return activeJiffies;
 }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  std::vector<string> CPU_jiffies = CpuUtilization();
  long idleJiffies = stol(CPU_jiffies[CPUStates::kIdle_]) + stol(CPU_jiffies[CPUStates::kIOwait_]);
  return idleJiffies;

}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  	std::string memName, size, line;
	std::vector<std::string> totalItem;
	std::ifstream stream(kProcDirectory + kStatFilename);
   		if (stream.is_open())
    	{
        	std::getline(stream, line);
        	std::istringstream linestream(line);
        	while (linestream >> memName >> size)
        	{
          		totalItem.push_back(memName);
          		totalItem.push_back(size);
        	}
        	totalItem.erase(totalItem.begin());
        
   	}
     stream.close();
  return totalItem; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
 int totalProcess = 0;
  std::string memName, unit, line;
  std::vector<std::string> memNames, sizes;
  std::ifstream stream(kProcDirectory+kStatFilename);
   if (stream.is_open()) 
   {
        while (std::getline(stream, line))
        {
        	std::istringstream linestream(line);
        	while(linestream >> memName >> totalProcess){
            	if(memName =="processes"){
                	return totalProcess;
                }
            }
    	}
  }
  return totalProcess; }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  int runningProcess = 0;
  std::string memName, unit, line;
  std::vector<std::string> memNames, sizes;
  std::ifstream stream(kProcDirectory+kStatFilename);
   if (stream.is_open()) 
   {
        while (std::getline(stream, line))
        {
        	std::istringstream linestream(line);
        	while(linestream >> memName >> runningProcess){
            	if(memName =="procs_running"){
                	return runningProcess;
                }
            }
    	}
  }

  return runningProcess; }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) 
{ 
  string command;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, command);
  }
  return command; 
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
   std::string memName,line, memValue;
  long unit;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
 if (stream.is_open()) {
    while (std::getline(stream, line))
    {
      std::istringstream linestream(line);
      linestream >> memName;
      if (memName == "VmSize:") 
      {
        linestream >> unit;
        unit /= SCALE_DOWN;
        memValue = std::to_string(unit);
        break;
      }
    }
  }  
  return memValue; }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
    // Read and return the user ID associated with a process
  string line, key, uid;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "Uid:") {
        linestream >> uid;
        break;
      }
    }
  }
  return uid;
  
  }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) 
{  
  string uid = Uid(pid);
  string id, x, temp, line;
  string name = "DEFAULT";
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) 
  {
      while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> temp >> x >> id;
      if (id == uid) {
        name = temp;
        break;
      }
    }
  }
  return name; }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) 
{  
  string key, line;
  long int value{0};
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for (int i = 0; i < 21; ++i) {
      linestream >> key;
    }
    linestream >> value;
    value = value / sysconf(_SC_CLK_TCK);
    return value;
  }
  return value; }
