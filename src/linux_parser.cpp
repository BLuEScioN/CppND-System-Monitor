#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
#include <numeric>
#include "linux_parser.h"

using std::cout;
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
  string os, version, kernel;
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

// Done: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
    string key;
    string mem_total;
    string mem_free;
  	string line;

    std::ifstream filestream(kProcDirectory + kMeminfoFilename);
    if (filestream.is_open()) {
        while (std::getline(filestream, line)) {
            std::istringstream linestream(line);
            linestream >> key;
            if (key == "MemTotal:") {
                linestream >> mem_total;
            } else if (key == "MemFree:") {
                linestream >> mem_free;
            }
        }
    }

    float mem_utilization = (stof(mem_total) - stof(mem_free)) / stof(mem_total);
    return mem_utilization; 
}

// Done: Read and return the system uptime
long LinuxParser::UpTime() { 
  	string uptime;
  	string line;
    
    std::ifstream filestream(kProcDirectory + kUptimeFilename);
    if (filestream.is_open()) {
        std::getline(filestream, line);
        std::istringstream linestream(line);
        linestream >> uptime;
    }

  	return stol(uptime);
}

// Done: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return LinuxParser::ActiveJiffies() + LinuxParser::IdleJiffies(); }

// Done: Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) { 
    std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
    vector<string> values;
    string line;
    string value;

    if (filestream.is_open()) {
        getline(filestream, line);
        std::istringstream stringstream(line);
        while (stringstream >> value) {
            values.push_back(value);
        }
    }

  	long total_time = 0;
    for (int i = 14; i <= 17; i++) {
        total_time += stol(values[i]);
    }

   return total_time;
}

// Done: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
    vector<string> jiffies = LinuxParser::CpuUtilization();    

    long active_jiffies = 
        stol(jiffies[CPUStates::kUser_]) + stol(jiffies[CPUStates::kNice_]) +
        stol(jiffies[CPUStates::kSystem_]) + stol(jiffies[CPUStates::kIRQ_]) +
        stol(jiffies[CPUStates::kSoftIRQ_]) + stol(jiffies[CPUStates::kSteal_]);

    return active_jiffies;
}

// Done: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
    vector<string> jiffies = LinuxParser::CpuUtilization();    

    long idle_jiffies = stol(jiffies[CPUStates::kIdle_]) + stol(jiffies[CPUStates::kIOwait_]);

    return idle_jiffies;    
}

// Done: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
    string key;
    string value;
    vector<string> jiffies;
  	string line;
  
    std::ifstream filestream(kProcDirectory + kStatFilename);
    if (filestream.is_open()) {
        while (std::getline(filestream, line)) {
            std::istringstream linestream(line);
            linestream >> key;
            if (key == "cpu") {
                while (linestream >> value) {
                    jiffies.push_back(value);
                }
                break;
            }
        };
    }

    return jiffies;
}

// Done: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
    string key;
    int total_processes;
  	string line;

    std::ifstream filestream(kProcDirectory + kStatFilename);
    if (filestream.is_open()) {
        while(std::getline(filestream, line)) {
            std::istringstream stringstream(line);
            stringstream >> key;
            if (key == "processes") {
                stringstream >> total_processes;
              	break;
            }
        }
    }

    return total_processes;
}

// Done: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
    string key;
    int procs_running;
  	string line;

    std::ifstream filestream(kProcDirectory + kStatFilename);
    if (filestream.is_open()) {
        while(std::getline(filestream, line)) {
            std::istringstream stringstream(line);
            stringstream >> key;
            if (key == "procs_running") {
                stringstream >> procs_running;
              	break;
            }
        }
    }

    return procs_running;
}

// Done: Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
    string line;
    std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
    if (filestream.is_open()) {
        getline(filestream, line);
    }
    return line;
}

// Done: Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
    string key;
    string ram;
  	string line;

    std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
    if (filestream.is_open()) {
        while (std::getline(filestream, line)) {
            std::istringstream linestream(line);
            linestream >> key;
            if (key == "VmSize:") {
                linestream >> ram;
                break;
            }
        }
    }

  	long ram_mb = stol(ram) / 1024;

    return to_string(ram_mb); 
}

// Done: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
    std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
    string key;
    string line;
    string uid;

    if (filestream.is_open()) {
        while (std::getline(filestream, line)) {
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

// Done: Read and return the user associated with a process
string LinuxParser::User(int pid) { 
    std::ifstream filestream(kPasswordPath);
    string line;
    string uid = Uid(pid);
    string x;
    string user = "-";
    string id;

    if (filestream.is_open()) {
        while (getline(filestream, line)) {
            std::replace(line.begin(), line.end(), ':', ' ');
            std::istringstream linestream(line);

            linestream >> user >> x >> id;
            if (id == uid) {
                return user;
            }
        }
    }
  
  	return user;
}

// Done: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) { 
    std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  	vector<string> values;
  	string value;
    string line;
    string uptime_string;
    if (filestream.is_open()) {
        getline(filestream, line);
        std::istringstream stringstream(line);
        while (stringstream >> value) {
           values.push_back(value);
        }
    }

    return UpTime() - stol(values[21]) / sysconf(_SC_CLK_TCK);
}