#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include <cmath>
#include <iostream>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) {
    this->pid = pid;

    string pid_file = "/proc/";
    pid_file += to_string(pid);
    pid_file += "/";
    this-> pid_file = pid_file;

}

// TODO DONE: Return this process's ID
int Process::Pid() { return this->pid; }

// TODO DONE: Return this process's CPU utilization
float Process::CpuUtilization() const {
    std::ifstream f(this->pid_file + "stat");
    std::string line;
    std::getline(f, line);
    std::stringstream line_stream(line);

    std::string a1;
    std::string a2;
    std::string a3;
    std::string a4;
    std::string a5;
    std::string a6;
    std::string a7;
    std::string a8;
    std::string a9;
    std::string a10;
    std::string a11;
    std::string a12;
    std::string a13;
    std::string a18;
    std::string a19;
    std::string a20;
    std::string a21;

    int a14;
    int a15;
    int a16;
    int a17;
    int a22;

    line_stream >> a1 >> a2 >> a3 >> a4 >> a5 >> a6 >> a7 >> a8 >> a9 >> a10
                >> a11 >> a12 >> a13 >> a14 >> a15 >> a16 >> a17 >> a18 >> a19 >> a20 >> a21 >> a22;

    // #14 utime - CPU time spent in user code, measured in clock ticks
    // #15 stime - CPU time spent in kernel code, measured in clock ticks
    // #16 cutime - Waited-for children's CPU time spent in user code (in clock ticks)
    // #17 cstime - Waited-for children's CPU time spent in kernel code (in clock ticks)
    // #22 starttime - Time when the process started, measured in clock ticks
    int utime = a14;
    int stime = a15;
    int cutime = a16;
    int cstime = a17;
    int starttime = a22;

    long hertz = sysconf(_SC_CLK_TCK);

    int total_time = utime + stime;
    total_time = total_time + cutime + cstime;

    long uptime = LinuxParser::UpTime();

    float seconds = (float)uptime - ((float)starttime / (float)hertz);

    float cpu_usage = ((float)total_time / (float)hertz) / seconds;

    return cpu_usage;
}

// TODO DONE: Return the command that generated this process
string Process::Command() {
    std::ifstream f(this->pid_file + "cmdline");
    string line;
    std::getline(f, line);
    return line;
}

// TODO DONE: Return this process's memory utilization
string Process::Ram() {
    std::ifstream f(this->pid_file + "status");
    string line;
    string key;
    int kb;
    int vmsize = 0;
    while (std::getline(f, line)) {
        std::stringstream line_stream(line);
        if (line_stream >> key >> kb) {
            if (key == "VmSize:") {
                vmsize = kb;
                break;
            }
        }
    }

    return to_string((int)floor((vmsize) / 1024.0));
}

// TODO DONE: Return the user (name) that generated this process
string Process::User() {
    std::ifstream f(this->pid_file + "status");
    string line;
    string key;
    string value;
    string user_id;
    string user_name = "";
    while (std::getline(f, line)) {
        std::stringstream line_stream(line);
        if (line_stream >> key >> value) {
            if (key == "Uid:") {
                user_id = value;
                break;
            }
        }
    }

    std::ifstream g("/etc/passwd");
    string a; string b; string c;
    while (std::getline(g, line)) {
        std::stringstream line_stream(line);
        std::getline(line_stream, a, ':');
        std::getline(line_stream, b, ':');
        std::getline(line_stream, c, ':');
        // std::cout << a << "aaaaa" << std::endl;
        if (c == user_id) {
            user_name = a;
            break;
        }
    }

    return user_name;

}

// TODO DONE: Return the age of this process (in seconds)
long int Process::UpTime() {
    std::ifstream f(this->pid_file + "stat");
    std::string line;
    std::getline(f, line);
    std::stringstream line_stream(line);

    std::string a1;
    std::string a2;
    std::string a3;
    std::string a4;
    std::string a5;
    std::string a6;
    std::string a7;
    std::string a8;
    std::string a9;
    std::string a10;
    std::string a11;
    std::string a12;
    std::string a13;
    std::string a18;
    std::string a19;
    std::string a20;
    std::string a21;

    int a14;
    int a15;
    int a16;
    int a17;
    int a22;

    line_stream >> a1 >> a2 >> a3 >> a4 >> a5 >> a6 >> a7 >> a8 >> a9 >> a10
                >> a11 >> a12 >> a13 >> a14 >> a15 >> a16 >> a17 >> a18 >> a19 >> a20 >> a21 >> a22;

    // #14 utime - CPU time spent in user code, measured in clock ticks
    // #15 stime - CPU time spent in kernel code, measured in clock ticks
    // #16 cutime - Waited-for children's CPU time spent in user code (in clock ticks)
    // #17 cstime - Waited-for children's CPU time spent in kernel code (in clock ticks)
    // #22 starttime - Time when the process started, measured in clock ticks
    int starttime = a22;

    long hertz = sysconf(_SC_CLK_TCK);

    long uptime = LinuxParser::UpTime();

    float res = (float)uptime - (float)starttime / (float)hertz;
    return floor(res);
}

// TODO DONE: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
    return this->CpuUtilization() > a.CpuUtilization();
}