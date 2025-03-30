#ifndef ANT_HEADER
#define ANT_HEADER

#include<vector>
#include<map>
#include <random>

#include "jobs.h"


class Ant{
  public:
    Ant();

    Ant(Job, int, std::map<Job, float>);

    Ant(Ant&); 

    void setJob(const Job);

    const Job getJob();

    void move(float);

    std::pair<float, float> getPosition();

    void encounterAnt(Ant);

    void evaluateEncountersAndSwitch();

  private:
    std::random_device _rd;
    std::pair<float, float> _position;
    Job _job;
    int _jobBufferSize;
    std::vector<Job> _encounteredJobs;
    std::map<Job, float> _desiredJobProportions;
};

#endif // !ANT_HEADER
