#ifndef ANTSIM_H
#define ANTSIM_H

#include "ant.h"
#include "jobs.h"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <map>
#include <vector>

class AntSim {
  public:
    AntSim(std::map<Job, float>, int, float, float, std::map<Job, sf::Color>, std::pair<int, int>);

    ~AntSim();
    
    void addAnt(Ant*);

    void removeAnts(std::pair<float, float>, float);
  
    void randomColony(int);

    int getColonySize();

    std::map<Job, int> getActualJobQuantities();

    void update(sf::RenderWindow&);

    void drawSim(sf::RenderWindow&, bool, Job, sf::Font&);

  private:
    void _moveAnts();
    void _evaluateAntEncounters();
    float _distanceBetweenAnts(Ant*, Ant*);
    float _euclideanDistance(std::pair<float, float>, std::pair<float, float>);
    void _storeAnt(Ant*, std::pair<float, float>);
    void _storeAnt(Ant*);
    void _removeAnt(Ant*, std::pair<int, int>);
    std::vector<std::vector<Ant*>> _getSurroundingAnts(Ant*);

    std::pair<int, int> _spaceDimensions;

    const float _maxMoveDist = 2.8;
    const float _antSize = 1.8;

    float _antInteractionDist;

    float _gridCellSize;
    int _gridCellsX;
    int _gridCellsY;

    sf::CircleShape _antCircle;
    sf::Text _spawningJobText;
    const int _spawningJobTextSize = 100;

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
