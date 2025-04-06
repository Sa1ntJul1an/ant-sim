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
    AntSim(std::map<Job, float>, int, float, float, std::map<Job, sf::Color>, std::pair<int, int>);

    ~AntSim();
    
    void addColony(std::vector<Ant*>);

    void randomColony(int);

    int getColonySize();

    std::map<Job, int> getActualJobQuantities();

    void update();

    void drawSim(sf::RenderWindow&);

  private:
    void _moveAnts();
    void _evaluateAntEncounters();
    float _distanceBetweenAnts(Ant*, Ant*);
    void _storeAnt(Ant*, std::pair<float, float>);
    void _storeAnt(Ant*);
    std::vector<std::vector<Ant*>> _getSurroundingAnts(Ant*);

    std::pair<int, int> _spaceDimensions;

    const float _maxMoveDist = 1.8;
    const float _antSize = 1.8;

    float _antInteractionDist;

    float _gridCellSize;
    int _gridCellsX;
    int _gridCellsY;

    sf::CircleShape _antCircle;

    std::vector<Ant*> _allAnts;
    std::map<std::pair<int, int>, std::vector<Ant*>> _partitionedAnts;

    std::map<Job, float> _idealJobProportions;
    std::map<Job, int> _actualJobQuantities;
    int _antEncounterBufferSize;

    std::map<Job, sf::Color> _jobColors;

    int _population;
    
    static std::random_device rd;
    static std::mt19937 gen;
};

#endif // ANTSIM_H
