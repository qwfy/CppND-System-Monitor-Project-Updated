#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
#include <cmath>

#include "linux_parser.h"

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
  string os, _version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> _version >> kernel;
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

// TODO DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
    std::ifstream f("/proc/meminfo");
    string total_line, _free_line, available_line;
    int total = 0;
    int available = 0;
    string ph;

    float utilization = 0;
    if (f.is_open()) {
        std::getline(f, total_line);
        std::getline(f, _free_line);
        std::getline(f, available_line);
        std::stringstream total_stream(total_line);
        std::stringstream available_stream(available_line);
        total_stream >> ph >> total;
        available_stream >> ph >> available;
        if (total > 0) {
            utilization = (float)(total - available) / (float)total;
        } else {
            std::cerr << "Cannot parse /proc/meminfo for memory utilization 3" << std::endl;
        }
    } else {
        std::cerr << "Cannot parse /proc/meminfo for memory utilization 1" << std::endl;
    }
    return utilization;
}

// TODO DONE: Read and return the system uptime
long LinuxParser::UpTime() {
    float uptime = 0;
    string line;
    std::ifstream f("/proc/uptime");
    std::getline(f, line);
    std::stringstream line_stream(line);
    line_stream >> uptime;
    return floor(uptime);
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO DONE: Read and return CPU utilization
LinuxParser::CPUStat LinuxParser::CpuUtilization() {
    // better if we use a optional
    int user = 0;
    int nice = 0;
    int system = 0;
    int idle = 0;
    int iowait = 0;
    int irq = 0;
    int softirq = 0;
    int steal = 0;
    int guest = 0;
    int guest_nice = 0;
    string line;
    string _cpu;
    std::ifstream stream("/proc/stat");
    if (stream.is_open()) {
        std::getline(stream, line);
        std::istringstream linestream(line);
        linestream >> _cpu
                   >> user >> nice >> system >> idle >> iowait
                   >> irq >> softirq >> steal >> guest >> guest_nice;
    } else {
        std::cerr << "Cannot parse /proc/stat for cpu utilization" << std::endl;
    }
    auto stat = LinuxParser::CPUStat {
            user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice
    };
    // std::cout << "stat.user is: " << stat.user << std::endl;
    return stat;
}

// TODO DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
    int total_processes = 0;
    std::ifstream f("/proc/stat");
    if (f.is_open()) {
        std::string line;
        std::string first;
        int second;
        while (std::getline(f, line)) {
            std::stringstream line_stream(line);
            if (line_stream >> first >> second) {
                if (first == "processes") {
                    total_processes = second;
                    break;
                }
            }
        }
    } else {
        std::cerr << "Cannot parse /proc/stat for total processes" << std::endl;
    }

    return total_processes;
}

// TODO DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
    int running_processes = 0;
    std::ifstream f("/proc/stat");
    if (f.is_open()) {
        std::string line;
        std::string first;
        int second;
        while (std::getline(f, line)) {
            std::stringstream line_stream(line);
            if (line_stream >> first >> second) {
                if (first == "procs_running") {
                    running_processes = second;
                    break;
                }
            }
        }
    } else {
        std::cerr << "Cannot parse /proc/stat for running processes" << std::endl;
    }

    return running_processes;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }