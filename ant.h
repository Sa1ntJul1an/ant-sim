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

    void setJob(const Job);

    const Job getJob();

    void move(float, std::pair<int, int>);

    std::pair<float, float> getPosition();

    void setPosition(std::pair<float, float>);

    void encounterAnt(Ant*);

    void evaluateEncountersAndSwitch();

  private:
    std::pair<float, float> _position;
    Job _job;
    int _jobBufferSize;
    std::vector<Job> _encounteredJobs;
    std::map<Job, float> _desiredJobProportions;
    
    static std::random_device rd;
    static std::mt19937 gen;
};

#endif // !ANT_HEADER
