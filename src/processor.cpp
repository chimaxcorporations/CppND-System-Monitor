#include "processor.h"
#include <chrono>   
#include <thread>   
#include "linux_parser.h"
// TODO: Return the aggregate CPU utilization
float Processor::Utilization()
{ 
float prevActiveJiffies =   LinuxParser::ActiveJiffies();
float totalPrevJiffies = prevActiveJiffies+LinuxParser::IdleJiffies();
std::this_thread::sleep_for (std::chrono::milliseconds(500));
float currentActiveJiffies = LinuxParser::ActiveJiffies();
float totalCurrentJiffies = LinuxParser::ActiveJiffies()+LinuxParser::IdleJiffies();
float currentCPUState = ((currentActiveJiffies-prevActiveJiffies)/(totalCurrentJiffies-totalPrevJiffies));
return currentCPUState; 
}