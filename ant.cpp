#include "ant.h"

#include <map>
#include <random>

#include "jobs.h"


Ant::Ant() {
  std::mt19937 gen(_rd());
  std::uniform_int_distribution<> dis(0, static_cast<int>(Job::NUM_JOBS) - 1);
  
  _job = static_cast<Job>(dis(gen));      // random ant job
  
  _jobBufferSize = 10;

  float equalPercentage = 1 / float(Job::NUM_JOBS);
  
  for (int i = 0; i < static_cast<int>(Job::NUM_JOBS); i++) {     // set all desired job proportions to equal value based on number of jobs
    _desiredJobProportions[static_cast<Job>(i)] = equalPercentage;
  }
}

Ant::Ant(Job job, int bufferSize, std::map<Job, float> desiredJobProportions) {
  _job = job;
  _jobBufferSize = bufferSize;
  _desiredJobProportions = desiredJobProportions;
}

void Ant::setJob(Job job) {
  _job = job;
}

const Job Ant::getJob() {
  return _job;
}

void Ant::move(float maxMoveDist, std::pair<int, int> dimensions) {
  std::uniform_int_distribution<> dis(-maxMoveDist, maxMoveDist);
  std::mt19937 gen(_rd());

  float width = static_cast<float>(dimensions.first);
  float height = static_cast<float>(dimensions.second);
  
  float positionX = _position.first;
  float positionY = _position.second;

  float moveX;
  float moveY;
  
  do {
    moveX = dis(gen);
    moveY = dis(gen);
  } while(positionX - moveX < 0.0 || positionY - moveY < 0.0 || positionX + moveX > width || positionY + moveY > height);
  
  _position.first += moveX;
  _position.second += moveY;
}

std::pair<float, float> Ant::getPosition() {
  return _position;
}

void Ant::encounterAnt(Ant encountered_ant) {
  _encounteredJobs.push_back(encountered_ant.getJob());

  if (_encounteredJobs.size() > _jobBufferSize) {
    _encounteredJobs.erase(_encounteredJobs.begin());     // ensure that encountered ants buffer doesn't exceed max size
  }
}

void Ant::evaluateEncountersAndSwitch() {
  if (_encounteredJobs.size() < _jobBufferSize) {
    return;                                               // we have not encountered enough ants to switch job
  }

  std::map<Job, int> jobCounts; 
  for (Job job : _encounteredJobs) {                      // tally up number of each job encountered
    jobCounts[job] ++;
  }

  std::map<Job, float> encounteredProportion;
  for (std::pair<Job, int> jobCountPair : jobCounts) {
    encounteredProportion[jobCountPair.first] = static_cast<float>(jobCountPair.second) / _jobBufferSize;
  }

  
}
