#include "processor.h"
#include "linux_parser.h"

// According to https://stackoverflow.com/a/23376195/7208627
//
// PrevIdle = previdle + previowait
// Idle = idle + iowait
//
// PrevNonIdle = prevuser + prevnice + prevsystem + previrq + prevsoftirq + prevsteal
// NonIdle = user + nice + system + irq + softirq + steal
//
// PrevTotal = PrevIdle + PrevNonIdle
// Total = Idle + NonIdle
//
// # differentiate: actual value minus the previous one
// totald = Total - PrevTotal
// idled = Idle - PrevIdle
//
// CPU_Percentage = (totald - idled)/totald

// TODO DONE: Return the aggregate CPU utilization
float Processor::Utilization() {
    LinuxParser::CPUStat stat = LinuxParser::CpuUtilization();

    int prev_idle = this->prev.idle + this->prev.iowait;
    int idle = stat.idle + stat.iowait;

    int prev_non_idle = this->prev.user + this->prev.nice + this->prev.system + this->prev.irq + this->prev.softirq + this->prev.steal;
    int non_idle = stat.user + stat.nice + stat.system + stat.irq + stat.softirq + stat.steal;

    int prev_total = prev_idle + prev_non_idle;
    int total = idle + non_idle;

    float total_d = total - prev_total;
    float idle_d = idle - prev_idle;

    float percentage = (total_d - idle_d) / total_d;

    this->prev = stat;
    return percentage;
}