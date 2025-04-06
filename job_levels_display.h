#ifndef JOB_LEVELS_DISPLAY_H
#define JOB_LEVELS_DISPLAY_H 

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <map>

#include "jobs.h"


class JobLevelsDisplay {
  public: 
    JobLevelsDisplay(int, std::map<Job, float>, std::map<Job, sf::Color>, int, int, sf::Font&);

    void drawDisplay(sf::RenderWindow&, int, std::map<Job, int>);

  private:
    const float _barVerticalPadding = 45.0;

    const float _barRectangleWidth = 10.0;
    const float _barBackgroundRectangleWidth = 12.0;

    const float _idealLevelRectangleHeight = 3.0;
    float _idealLevelRectangleWidth = _barRectangleWidth + 20.0;

    const sf::Color _barBackgroundColor = sf::Color(70, 70, 70);

    float _windowWidth, _windowHeight;

    int _numJobs;

    std::map<Job, float> _idealJobLevels; 
    
    std::map<Job, sf::Color> _jobColors;

    sf::Font _font;
    sf::Text _jobNameText;

    sf::RectangleShape _barBackgroundRectangle;
    sf::RectangleShape _barRectangle;; 
    sf::RectangleShape _idealLevelRectangle;
};

#endif // !JOB_LEVELS_DISPLAY_H
