#include "antsim.h"

#include "ant.h"
#include "jobs.h"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <math.h>
#include <map>
#include <utility>
#include <vector>
#include <random>
#include <iostream>

AntSim::AntSim(std::map<Job, float> idealJobProportions, int antEncounterBufferSize, float antInteractionDist, float gridCellSize, std::map<Job, sf::Color> jobColors, std::pair<int, int> dimensions) {
  _spaceDimensions = dimensions;

  _idealJobProportions = idealJobProportions;
  _antEncounterBufferSize = antEncounterBufferSize;
  
  _antInteractionDist = antInteractionDist;

  _gridCellSize = gridCellSize;
  _gridCellsX = _spaceDimensions.first / _gridCellSize;
  _gridCellsY = _spaceDimensions.second / _gridCellSize;

  _jobColors = jobColors;

  _antCircle.setRadius(_antSize); 
}

AntSim::~AntSim() {
  for (Ant* ant : _allAnts) {
    delete ant;
  }
}

/*void AntSim::addColony(std::vector<Ant*> ants) {*/
/*  _ants = ants;*/
/*  _population = _ants.size();*/
/*}*/

void AntSim::randomColony(int populationSize) {
  _population = populationSize;

  std::uniform_int_distribution<> dis(0.0f, 1000.0f);

  std::vector<float> randomJobProportions;

  for (int i = 0; i < static_cast<int>(Job::NUM_JOBS); i++) {    // for each different job type, create a random percentage between 0 and 1
    randomJobProportions.push_back(dis(gen));
  }

  float jobProportionsSum;
  for (float proportion : randomJobProportions) {                 // sum randomly generated proportions
    jobProportionsSum += proportion;
  }

  for (int i = 0; i < static_cast<int>(Job::NUM_JOBS); i++) {     // normalize so that all proportions sum to 1
    randomJobProportions[i] /= jobProportionsSum;
  }

  std::vector<Ant*> ant_buffer;
  int sumOfAnts = 0;
  for (int i = 0; i < static_cast<int>(Job::NUM_JOBS); i++) {
    Job job = static_cast<Job>(i);
    float randomProportion = randomJobProportions[i];

    int antsWithjob;

    if (i == static_cast<int>(Job::NUM_JOBS) - 1) {
      antsWithjob = populationSize - sumOfAnts;
    } else {
      antsWithjob = std::round(populationSize * randomProportion);
      sumOfAnts += antsWithjob;
    }

    std::cout << "Ants with job " << getJobName(job) << " : " << antsWithjob << std::endl;

    for (int antIndex = 0; antIndex < antsWithjob; antIndex++) {
      Ant* ant = new Ant(job, _antEncounterBufferSize, _idealJobProportions);
      ant_buffer.push_back(ant);
    }
  }
  
  std::cout << "Placing ants in environment...\n";
  for (Ant* ant : ant_buffer) {  // place ants down randomly in state space
    
    std::uniform_int_distribution<> x_dist(0.0f, _spaceDimensions.first);
    std::uniform_int_distribution<> y_dist(0.0f, _spaceDimensions.second);

    std::pair<float, float> ant_pos; 
    bool colliding;

    int max_attempts = 20;
    int current_attempt = 0;
    bool attempts_exceeded;
    do {
      attempts_exceeded = false;
      current_attempt++;
      
      colliding = false;

      ant_pos.first = x_dist(gen);
      ant_pos.second = y_dist(gen);

      ant->setPosition(ant_pos);

      if (_allAnts.size() == 0) {
        break;
      }

      std::vector<std::vector<Ant*>> surroundingAnts = _getSurroundingAnts(ant);

      for (std::vector<Ant*> ants : surroundingAnts) {
        for (Ant* other_ant : ants) {
          if (other_ant == ant) {
            continue;
          }

          if (_distanceBetweenAnts(ant, other_ant) < _antSize) {
            colliding = true;
          }
        }
      }
      
      if (current_attempt > max_attempts) {
        attempts_exceeded = true;
        std::cout << "Exceeded max retries when attempting to find a place to spawn ant. Colony size may be too great for environment.\n";
      }

    } while(colliding);
    
    if (!attempts_exceeded) {
      _storeAnt(ant);
      _allAnts.push_back(ant);
    }
  }
  std::cout << "Ant colony created.  Population size: " << _allAnts.size() << std::endl;
}

int AntSim::getColonySize() {
  return _allAnts.size();
}

std::map<Job, int> AntSim::getActualJobQuantities() {
  return _actualJobQuantities; 
}

void AntSim::update() {
  _evaluateAntEncounters();
  
  std::map<Job, int> antsWithJob;
  for (Ant* ant : _allAnts) {
    // run function for each ant to evaluate how many of each job it has encountered and switch accordingly
    ant->evaluateEncountersAndSwitch();

    ant->move(_maxMoveDist, _spaceDimensions);
    antsWithJob[ant->getJob()]++;
  }

  for (int i = 0; i < static_cast<int>(Job::NUM_JOBS); i++) {
    Job job = static_cast<Job>(i);
    _actualJobQuantities[job] = antsWithJob[job];
  }
}

void AntSim::drawSim(sf::RenderWindow& renderWindow) {
  for (Ant* ant : _allAnts) {
    std::pair<float, float> antPosition = ant->getPosition();
    Job job = ant->getJob();

    _antCircle.setPosition(sf::Vector2f(antPosition.first - _antSize, antPosition.second - _antSize));
    _antCircle.setFillColor(_jobColors[job]);

    renderWindow.draw(_antCircle);
  }
}

void AntSim::_evaluateAntEncounters() {
  // for each ant 
  for (Ant* ant1 : _allAnts) {
    // get vectors of neighboring ants
    std::vector<std::vector<Ant*>> surroundingAnts = _getSurroundingAnts(ant1);
    // for each vector of ants in neighboring grid cells
    for (std::vector<Ant*> antsInGridCell : surroundingAnts) {
      // for each surrounding ant, check for encounter
      for (Ant* ant2 : antsInGridCell) {
        if (ant1 == ant2) {
          continue;
        }
        if (_distanceBetweenAnts(ant1, ant2) < _antInteractionDist) {
          ant1->encounterAnt(ant2);
          ant2->encounterAnt(ant1);
        }
      }
    }
  }
}

float AntSim::_distanceBetweenAnts(Ant* ant1, Ant* ant2) {
  if (ant1 == ant2) {
    return 0.0f;
  }

  std::pair<float, float> ant1_pos = ant1->getPosition();
  std::pair<float, float> ant2_pos = ant2->getPosition();

  float delta_x = abs(ant1_pos.first - ant2_pos.first);
  float delta_y = abs(ant1_pos.second - ant2_pos.second);
  
  return sqrt(pow(delta_x, 2) + pow(delta_y, 2));
}

void AntSim::_storeAnt(Ant* ant, std::pair<float, float> previous_position) {   // used to store an ant that has moved (has a previous position)
  int grid_location_x = ant->getPosition().first / _gridCellSize;
  int grid_location_y = ant->getPosition().second / _gridCellSize;
  
  int prev_grid_location_x = previous_position.first / _gridCellSize;
  int prev_grid_location_y = previous_position.second / _gridCellSize;

  // generate key based on ants location
  std::pair<int, int> grid_key = {grid_location_x, grid_location_y};
  
  // if ant has changed grid cells, we need to move it to a different vector of ants
  if (grid_location_x != prev_grid_location_x || grid_location_y != prev_grid_location_y) {
    // add it to vector of ants at correct grid cell
    _partitionedAnts[grid_key].push_back(ant);
    
    // grid key for ants previous position to delete it from previous vector of ants
    grid_key = {prev_grid_location_x, prev_grid_location_y};

    // we need to ensure the last ant is the one we want to delete so that we can easily remove it very efficiently
    if (_partitionedAnts[grid_key].back() != ant) {
      for (auto it = _partitionedAnts[grid_key].begin(); it != _partitionedAnts[grid_key].end(); ++it) {
        if (*it != nullptr && *it == ant) {
          std::swap(*it, _partitionedAnts[grid_key].back());
        }
      }
    }
    _partitionedAnts[grid_key].pop_back();
  }
}

void AntSim::_storeAnt(Ant* ant) {  // used to store a new ant that has not been added to the colony yet
  int grid_location_x = ant->getPosition().first / _gridCellSize;
  int grid_location_y = ant->getPosition().second / _gridCellSize;

  // generate key based on ants location
  std::pair<int, int> grid_key = {grid_location_x, grid_location_y};
  
  // add it to vector of ants at correct grid cell
  _partitionedAnts[grid_key].push_back(ant);
}

std::vector<std::vector<Ant*>> AntSim::_getSurroundingAnts(Ant* ant) {
  int cell_x = static_cast<int>(ant->getPosition().first / _gridCellSize);
  int cell_y = static_cast<int>(ant->getPosition().second / _gridCellSize);

  std::pair<int, int> gridKey;
  std::vector<std::vector<Ant*>> ants;

  for (int x_offset = -1; x_offset <= 1; x_offset++) {
    if (x_offset < 0 || x_offset > _gridCellsX - 1) {
      continue;
    }
    for (int y_offset = -1; y_offset <= 1; y_offset++) {
      if (y_offset < 0 || y_offset > _gridCellsY - 1) {
        continue;
      }
      gridKey = {cell_x, cell_y};
      ants.push_back(_partitionedAnts[gridKey]);
    }
  }
  return ants;
}


std::random_device AntSim::rd;
std::mt19937 AntSim::gen(AntSim::rd());
