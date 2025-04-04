#include "jobs.h"

std::string getJobName(Job job) {
  std::string jobName;
  switch (job) {
    case Job::Guard:
      jobName = "Guard";
      break;
    case Job::Scout:
      jobName = "Scout";
      break;
    case Job::Gatherer:
      jobName = "Gatherer";
      break;
    case Job::Tunneler:
      jobName = "Tunneler";
      break;
    case Job::Caretaker:
      jobName = "Caretaker";
      break;
    default: 
      jobName = "Undefined";
      break;
  }
  return jobName;
}
