#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

#include "process.h"
#include "processor.h"
#include "system.h"

#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;



// TODO DONE: Return the system's CPU
Processor& System::Cpu() {
  return this->cpu_;
}

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() {
  auto pids = LinuxParser::Pids();

  this->processes_.clear();
  for (auto pid: pids) {
    this->processes_.push_back(Process(pid));
  }
  std::sort(this->processes_.begin(), this->processes_.end());

  return this->processes_;
}

// TODO DONE: Return the system's kernel identifier (string)
std::string System::Kernel() {
    return LinuxParser::Kernel();
}

// TODO DONE: Return the system's memory utilization
float System::MemoryUtilization() {
  return LinuxParser::MemoryUtilization();
}

// TODO DONE: Return the operating system name
std::string System::OperatingSystem() {
    return LinuxParser::OperatingSystem();
}

// TODO DONE: Return the number of processes actively running on the system
int System::RunningProcesses() {
  return LinuxParser::RunningProcesses();
}

// TODO DONE: Return the total number of processes on the system
int System::TotalProcesses() {
  return LinuxParser::TotalProcesses();
}

// TODO DONE: Return the number of seconds since the system started running
long int System::UpTime() {
  return LinuxParser::UpTime();
}