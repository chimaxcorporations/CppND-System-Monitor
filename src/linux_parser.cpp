#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include "linux_parser.h"
#define SCALE_DOWN 1000
#define MAX_JIF_COUNT 4
#define MAX_MEM_COUNT 5

using std::stof;
using std::string;
using std::to_string;
using std::vector;
template <typename T>
T LinuxParser::findValue(std::string const &filename){
   std::string line{"null"}, key{"null"};
std::ifstream stream(kProcDirectory + filename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> key;
    return key;
  }
return key;
}
template <typename T>
T LinuxParser::findValueByKey(std::string const &keyFilter, std::string  const &filename){
  std::string line, key;
  T value ;
  std::ifstream stream(kProcDirectory + filename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == keyFilter) {
          return value;
        }
      }
    }
  }
  return value;
}
template <typename T>
T LinuxParser::getValueOfFile(std::string const &filename) {
  std::string line;
  T value;
  std::ifstream stream(kProcDirectory + filename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> value;
  }
  return value;
}
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
        pids.emplace_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Calculate memory utilization - Done
float LinuxParser::MemoryUtilization() {
  string memTotal = "MemTotal:";
  string memFree = "MemFree:";
  float Total = findValueByKey<float>(memTotal, kMeminfoFilename);// "/proc/memInfo"
  float Free = findValueByKey<float>(memFree, kMeminfoFilename);
  return (Total - Free) / Total;
}

// Done
long LinuxParser::UpTime() { 
return stol(findValue<std::string>(kUptimeFilename));
 }

//Done
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

//Done
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
    return active_jiffies/sysconf(_SC_CLK_TCK);
  }
  return active_jiffies/sysconf(_SC_CLK_TCK); }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() 
{
    std::vector<string> CPU_jiffies = CpuUtilization();
  long activeJiffies = stol(CPU_jiffies[CPUStates::kUser_]) + stol(CPU_jiffies[CPUStates::kNice_]) +stol(CPU_jiffies[CPUStates::kSystem_]) + 		 	stol(CPU_jiffies[CPUStates::kIRQ_]) + stol(CPU_jiffies[CPUStates::kSoftIRQ_]) +stol(CPU_jiffies[CPUStates::kSteal_]); 
   return activeJiffies;
 }

//Done
long LinuxParser::IdleJiffies() { 
  std::vector<string> CPU_jiffies = CpuUtilization();
  long idleJiffies = stol(CPU_jiffies[CPUStates::kIdle_]) + stol(CPU_jiffies[CPUStates::kIOwait_]);
  return idleJiffies;

}

//Done
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



// Done
int LinuxParser::TotalProcesses() { 
 string process{"processes"};
 return findValueByKey<int>(process,kStatFilename);
}


// Done
int LinuxParser::RunningProcesses() {
  string runningProcess{"procs_running"};
  return findValueByKey<int>(runningProcess,kStatFilename);
}

//Doone
string LinuxParser::Command(int pid) {
  return std::string(getValueOfFile<std::string>(std::to_string(pid) + kCmdlineFilename));
}
//Done
string LinuxParser::Ram(int pid) { 
  string keyFilter{"VmRSS"};
  string filename = std::to_string(pid) + kStatusFilename;
  int value = findValueByKey<double>(keyFilter,filename);
  return to_string(value);
}


//Done
string LinuxParser::Uid(int pid)
{
  string line, key, keyFilter{"Uid:"};
  string filename =  to_string(pid) + kStatusFilename;
  return findValueByKey<string>(keyFilter, filename);
}
  

//Done
string LinuxParser::User(int pid) 
{  
  string uid = Uid(pid);
  string id, x,line, user;
  string name = "DEFAULT";
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) 
  {
      while (std::getline(stream, line)) 
      {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> name >> x >> id)
      {
      	if (id == uid)
        {
        	return name;
      	}
      }
    }
  }
  return user; 
}

//Done
long LinuxParser::UpTime(int pid) 
{  
  string key, value,line;
  long  upTimePid = 0;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    for (int i = 0; i < 21; ++i) {
      linestream >> key;
    }
    linestream >> value;
    upTimePid = UpTime() - stol(value)/sysconf(_SC_CLK_TCK);
    return upTimePid;
  }
  return upTimePid;
}

