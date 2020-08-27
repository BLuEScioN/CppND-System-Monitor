#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "linux_parser.h"

class Processor {
 public:
  float Utilization();  
  
  float GetPrevIdle() { return prev_idle_; };
  float GetPrevActive() { return prev_active_; };

  float GetCurrIdle() { return float(LinuxParser::IdleJiffies()); };
  float GetCurrActive() { return float(LinuxParser::ActiveJiffies()); };
  
  void SetPrevIdle(float curr_idle) { prev_idle_ = curr_idle; };
  void SetPrevActive(float curr_active) { prev_active_ = curr_active; };

  // Done: Declare any necessary private members
 private:
    float prev_idle_;
    float prev_active_;
};

#endif