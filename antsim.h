#ifndef ANTSIM_H
#define ANTSIM_H

#include "ant.h"
#include "jobs.h"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <map>
#include <vector>

class AntSim {
  public:
    AntSim(std::map<Job, float>, int, std::map<Job, sf::Color>, std::pair<int, int>);

    ~AntSim();
    
    void addColony(std::vector<Ant*>);

    void randomColony(int);

    void update();

    void drawSim(sf::RenderWindow&);

  private:
    void _evaluateAntEncounters();
    float _distanceBetweenAnts(Ant*, Ant*);

    std::pair<int, int> _spaceDimensions;

    const float _maxMoveDist = 1.8;
    const float _antSize = 1.0;
    
    sf::CircleShape _antCircle;

    std::vector<Ant*> _ants; 

    std::map<Job, float> _idealJobProportions;
    int _antEncounterBufferSize;

    std::map<Job, sf::Color> _jobColors;

    int _population;
    
    static std::random_device rd;
    static std::mt19937 gen;
};

#endif // ANTSIM_H
