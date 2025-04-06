#include "ant.h"

#include <cmath>
#include <map>
#include <random>

#include "jobs.h"


Ant::Ant() {
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
  float positionX = _position.first;
  float positionY = _position.second;

  float width = static_cast<float>(dimensions.first);
  float height = static_cast<float>(dimensions.second);

  float move_dist_x_lower_limit = maxMoveDist;
  float move_dist_x_upper_limit = maxMoveDist;
  float move_dist_y_lower_limit = maxMoveDist;
  float move_dist_y_upper_limit = maxMoveDist;

  if (positionX - move_dist_x_lower_limit < 0.0) {
    move_dist_x_lower_limit = positionX;
  }
  if (positionY - move_dist_y_lower_limit < 0.0) {
    move_dist_y_lower_limit = positionY;
  }
  if (positionX + move_dist_x_upper_limit > width) {
    move_dist_x_upper_limit = width - positionX;
  }
  if (positionY + move_dist_y_upper_limit > height) {
    move_dist_y_upper_limit = height- positionY;
  }

  std::uniform_real_distribution<> move_distribution_x(-move_dist_x_lower_limit, move_dist_x_upper_limit);
  std::uniform_real_distribution<> move_distribution_y(-move_dist_y_lower_limit, move_dist_y_upper_limit);
  
  float moveX;
  float moveY;
  
  moveX = move_distribution_x(gen);
  moveY = move_distribution_y(gen);

  _position.first += moveX;
  _position.second += moveY;
}

std::pair<float, float> Ant::getPosition() {
  return _position;
}

void Ant::encounterAnt(Ant* encountered_ant) {
  _encounteredJobs.push_back(encountered_ant->getJob());

  if (_encounteredJobs.size() > _jobBufferSize) {
    _encounteredJobs.erase(_encounteredJobs.begin());     // ensure that encountered ants buffer doesn't exceed max size
  }
}

void Ant::setPosition(std::pair<float, float> position) {
  _position = position;
}

void Ant::evaluateEncountersAndSwitch() {
  if (_encounteredJobs.size() < _jobBufferSize) {
    return;                                               // we have not encountered enough ants to switch job
  }

  std::map<Job, int> jobCounts; 
  for (Job job : _encounteredJobs) {                      // tally up number of each job encountered
    jobCounts[job] ++;
  }

  std::map<Job, float> encounteredProportions;
  for (std::pair<Job, int> jobCountPair : jobCounts) {
    encounteredProportions[jobCountPair.first] = static_cast<float>(jobCountPair.second) / _jobBufferSize;
  }

  std::vector<Job> underRepresentedJobs;
  std::vector<Job> overRepresentedJobs;
  for (std::pair<Job, float> encounteredProportion : encounteredProportions) {                  // - for each Job and encountered proportion
    Job job = encounteredProportion.first;
    float jobProportion = encounteredProportion.second;
    if (jobProportion < _desiredJobProportions[job]) {       // - if encountered proportion less than desired proportion
      underRepresentedJobs.push_back(job);                                             // - add Job to list of jobs that ant may switch to 
    } else {
      overRepresentedJobs.push_back(job);
    }
  }

  std::uniform_int_distribution<> dis(0, underRepresentedJobs.size() - 1);
  
  bool antInOverRepresentedJob = false;
  for (Job job : overRepresentedJobs) {
    if (_job == job) {
      antInOverRepresentedJob = true;
      break;
    }
  }

  // choose random job from list of potential jobs
  if (antInOverRepresentedJob && !underRepresentedJobs.empty()) {   // if ant detects that it is in an over represented job, and there are potential jobs to switch to 
    _job = underRepresentedJobs[dis(gen)];
  }
}


std::random_device Ant::rd;
std::mt19937 Ant::gen(Ant::rd());
