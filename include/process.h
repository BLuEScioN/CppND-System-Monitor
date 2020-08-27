#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid);
  int Pid() const;                              
  std::string User() const;                    
  std::string Command() const;                   
  float CpuUtilization() const;               
  std::string Ram() const;                       
  long int UpTime() const;                      
  bool operator<(Process const& a) const;  
  bool operator>(Process const& a) const;

  // Done: Declare any necessary private members
 private:
  int pid_;
  float cpu_utilization_;
  std::string ram_;
  long int uptime_;
  std::string command_;
  std::string user_;
};

#endif