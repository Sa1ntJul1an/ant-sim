#ifndef JOBS_H
#define JOBS_H

#include <string>
enum class Job;
/*
* Defines the different tasks that ants can have in the colony
*/

enum class Job {
  Gatherer,
  Scout,
  Guard,
  Caretaker,
  Tunneler,

  NUM_JOBS    // last element to easily get number of jobs
};

std::string getJobName(Job job);

#endif // !JOBS_H
