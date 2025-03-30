#ifndef JOBS_H
#define JOBS_H

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

#endif // !JOBS_H
