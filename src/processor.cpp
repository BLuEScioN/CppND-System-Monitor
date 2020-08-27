#include <unistd.h>
#include "processor.h"
#include "linux_parser.h"
#include <string>
#include <vector>

using std::vector;
using std::string;

// Done: Return the aggregate CPU utilization
// Linux stores processor utilization data within the /proc/stat file.
// you only need to display aggregate CPU information, which you can find on the "cpu" line of /proc/stat.
// If you would like to add individual processor information to your system monitor project, go for it!
// /proc/stat contains 10 integer values for each processor.
// user: normal processes executing in user mode
// nice: niced processes executing in user mode
// system: processes executing in kernel mode
// idle: twiddling thumbs
// iowait: In a word, iowait stands for waiting for I/O to complete. But there are several problems:
// Cpu will not wait for I/O to complete, iowait is the time that a task is waiting for I/O to complete. When cpu goes into idle state for outstanding task io, another task will be scheduled on this CPU.
// In a multi-core CPU, the task waiting for I/O to complete is not running on any CPU, so the iowait of each CPU is difficult to calculate.
// The value of iowait field in /proc/stat will decrease in certain conditions. So, the iowait is not reliable by reading from /proc/stat.
// irq: servicing interrupts
// softirq: servicing softirqs
// steal: involuntary wait
// guest: running a normal guest
// guest_nice: running a niced guest
float Processor::Utilization() { 
    float curr_idle = GetCurrIdle();
    float curr_active = GetCurrActive();

    float prev_idle = GetPrevIdle();
    float prev_active = GetPrevActive();

    SetPrevActive(curr_active);
    SetPrevIdle(curr_idle);

    float prev_total = prev_idle + prev_active;
    float curr_total = curr_idle + curr_active;

    float total_d = curr_total - prev_total;
    float idle_d = curr_idle - prev_idle;

    float cpu_percentage = (total_d - idle_d) / total_d; 

    return cpu_percentage;
}