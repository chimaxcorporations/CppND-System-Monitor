#include "processor.h"
#include <chrono>   
#include <thread>   
#include "linux_parser.h"
// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
long prevActiveJiffies = LinuxParser::ActiveJiffies();
long prevIdleJiffies = LinuxParser::IdleJiffies();
std::this_thread::sleep_for (std::chrono::seconds(1));
long currentActiveJiffies = LinuxParser::ActiveJiffies();
long currentIdleJiffies = LinuxParser::IdleJiffies();
long prevCPUState = prevActiveJiffies/(prevActiveJiffies+prevIdleJiffies);
long currentCPUState = currentActiveJiffies/(currentActiveJiffies+currentIdleJiffies);
float cpuAggregate  = currentCPUState - prevCPUState;
  return cpuAggregate; }