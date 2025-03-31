#include "antsim.h"

#include "ant.h"
#include "jobs.h"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <cmath>
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

  std::random_device rd;
  std::mt19937_64 gen(rd());
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
      _ants.push_back(ant);
    }
  }

  std::cout << "Ant colony created.  Population size: " << _ants.size() << std::endl;
}

void AntSim::update() {

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
