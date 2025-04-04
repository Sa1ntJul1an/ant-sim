#include "antsim.h"

#include "ant.h"
#include "jobs.h"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <math.h>
#include <map>
#include <vector>
#include <random>
#include <iostream>

AntSim::AntSim(std::map<Job, float> idealJobProportions, int antEncounterBufferSize, std::map<Job, sf::Color> jobColors, std::pair<int, int> dimensions) {
  _spaceDimensions = dimensions;

  _idealJobProportions = idealJobProportions;
  _antEncounterBufferSize = antEncounterBufferSize;
  _jobColors = jobColors;

 _antCircle.setRadius(_antSize); 
}

AntSim::~AntSim() {
  for (Ant* ant : _ants) {
    delete ant;
  }
}

void AntSim::addColony(std::vector<Ant*> ants) {
  _ants = ants;
  _population = _ants.size();
}

void AntSim::randomColony(int populationSize) {
  _population = populationSize;

  std::mt19937_64 gen(_rd());
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
    std::cout << "Ants with job " << i << " : " << antsWithjob << std::endl;

    for (int antIndex = 0; antIndex < antsWithjob; antIndex++) {
      Ant* ant = new Ant(job, _antEncounterBufferSize, _idealJobProportions);
      ant_buffer.push_back(ant);
    }
  }
  
  std::cout << "Placing ants in environment...\n";
  for (Ant* ant : ant_buffer) {  // place ants down randomly in state space
    std::mt19937_64 gen(_rd());
    
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

      if (_ants.size() <= 1) {
        break;
      }

      for (Ant* other_ant : _ants) {
        if (other_ant == ant) {
          continue;
        }

        if (_distanceBetweenAnts(ant, other_ant) < _antSize) {
          std::cout << "ants colliding. Distance" << _distanceBetweenAnts(ant, other_ant) << "\n";
          colliding = true;
        }
      }
      
      if (current_attempt > max_attempts) {
        attempts_exceeded = true;
        std::cout << "Exceeded max retries when attempting to find a place to spawn ant. Colony size may be too great for environment size.\n";
      }

    } while(colliding);
    
    ant->setPosition(ant_pos);
    
    if (!attempts_exceeded) {
      _ants.push_back(ant);
    }
  }
  std::cout << "Ant colony created.  Population size: " << _ants.size() << std::endl;
}

void AntSim::update() {
  for (Ant* ant : _ants) {
    ant->move(_maxMoveDist, _spaceDimensions);
  }
}

void AntSim::drawSim(sf::RenderWindow& renderWindow) {
  for (Ant* ant : _ants) {
    std::pair<float, float> antPosition = ant->getPosition();
    Job job = ant->getJob();

    _antCircle.setPosition(sf::Vector2f(antPosition.first - _antSize, antPosition.second - _antSize));
    _antCircle.setFillColor(_jobColors[job]);

    renderWindow.draw(_antCircle);
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
