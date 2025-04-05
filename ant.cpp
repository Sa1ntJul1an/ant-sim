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

  std::uniform_int_distribution<> move_distribution_x(std::round(-move_dist_x_lower_limit * 10), std::round(move_dist_x_upper_limit * 10));
  std::uniform_int_distribution<> move_distribution_y(std::round(-move_dist_y_lower_limit * 10), std::round(move_dist_y_upper_limit * 10));
  
  float moveX;
  float moveY;
  
  moveX = static_cast<float>(move_distribution_x(gen) / 10.0);
  moveY = static_cast<float>(move_distribution_y(gen) / 10.0);

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

  std::map<Job, float> encounteredProportion;
  for (std::pair<Job, int> jobCountPair : jobCounts) {
    encounteredProportion[jobCountPair.first] = static_cast<float>(jobCountPair.second) / _jobBufferSize;
  }
}


std::random_device Ant::rd;
std::mt19937 Ant::gen(Ant::rd());
