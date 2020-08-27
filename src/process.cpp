#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) {
    pid_ = pid;
    ram_ = LinuxParser::Ram(pid);
    uptime_ = LinuxParser::UpTime(pid);
    command_ = LinuxParser::Command(pid);
    user_ = LinuxParser::User(pid);

    float active_jiffies = LinuxParser::ActiveJiffies(pid);
    long seconds = LinuxParser::UpTime() - uptime_;
    
    cpu_utilization_ = (float(active_jiffies) / sysconf(_SC_CLK_TCK)) / float(seconds); 
}

// Done: Return this process's ID
int Process::Pid() const { return pid_; }

// Done: Return this process's CPU utilization
float Process::CpuUtilization() const { return cpu_utilization_; }

// Done: Return the command that generated this process
string Process::Command() const { return command_; }

// Done: Return this process's memory utilization
string Process::Ram() const { return ram_; }

// Done: Return the user (name) that generated this process
string Process::User() const { return user_; }

// Done: Return the age of this process (in seconds)
long int Process::UpTime() const { return uptime_; }

// Done: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const { 
 return CpuUtilization() < a.CpuUtilization();
 // return Ram() < a.Ram();
}

bool Process::operator>(Process const& a) const { 
 return CpuUtilization() > a.CpuUtilization();
 // return Ram() > a.Ram();
}