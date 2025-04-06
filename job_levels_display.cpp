#include "job_levels_display.h"

#include "jobs.h"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <string>


JobLevelsDisplay::JobLevelsDisplay(int colonySize, std::map<Job, float> idealJobProportions, std::map<Job, sf::Color> jobColors, int windowWidth, int windowHeight, sf::Font& font) : _font(font) {
  _idealJobLevels = idealJobProportions;
  _jobColors = jobColors;
  
  _windowWidth = static_cast<float>(windowWidth);
  _windowHeight = static_cast<float>(windowHeight);

  _numJobs = static_cast<int>(Job::NUM_JOBS);

  _jobNameText.setFont(_font);
  _jobNameText.setCharacterSize(12);
  _jobNameText.setFillColor(sf::Color::White);
  
  _idealJobQuantityText.setFont(_font);
  _idealJobQuantityText.setCharacterSize(12);
  _idealJobQuantityText.setFillColor(sf::Color::White);

  _actualJobQuantityText.setFont(_font);
  _actualJobQuantityText.setCharacterSize(12);

  _idealLevelRectangle.setFillColor(sf::Color(80, 80, 80));
  _idealLevelRectangle.setSize(sf::Vector2f(_idealLevelRectangleWidth, _idealLevelRectangleHeight));

  _barBackgroundRectangle.setSize(sf::Vector2f(_barBackgroundRectangleWidth, _windowHeight - _barVerticalPadding * 2.0));
  _barBackgroundRectangle.setFillColor(_barBackgroundColor);

  _barRectangle.setSize(sf::Vector2f(_barRectangleWidth, _windowHeight - _barVerticalPadding)); 
}

void JobLevelsDisplay::drawDisplay(sf::RenderWindow& window, int colonySize, std::map<Job, int> actualJobQuantites) {
  float sectionWidth = _windowWidth / static_cast<float>(_numJobs);

  for (int i = 0; i < _numJobs; i++) {
    Job job = static_cast<Job>(i);
    std::string jobName = getJobName(job);

    float idealJobLevel = _idealJobLevels[job];
    float actualJobLevel = static_cast<float>(actualJobQuantites[job]) / colonySize;
    
    float x_center = 0.5 * sectionWidth + sectionWidth * static_cast<float>(i);
    
    float ideal_level_y = _barVerticalPadding + _barBackgroundRectangle.getLocalBounds().height + _barBackgroundRectangle.getLocalBounds().top - idealJobLevel * _barBackgroundRectangle.getLocalBounds().height;
    _idealLevelRectangle.setPosition(sf::Vector2f(x_center - _idealLevelRectangleWidth / 2.0, ideal_level_y));

    float actual_level_y = _barVerticalPadding + _barBackgroundRectangle.getLocalBounds().height + _barBackgroundRectangle.getLocalBounds().top - actualJobLevel * _barBackgroundRectangle.getLocalBounds().height;
    _barRectangle.setFillColor(_jobColors[job]);
    _barRectangle.setSize(sf::Vector2f(_barRectangleWidth, _barBackgroundRectangle.getLocalBounds().height * actualJobLevel));
    _barRectangle.setPosition(sf::Vector2f(x_center - _barRectangleWidth / 2.0, actual_level_y));

    _barBackgroundRectangle.setPosition(sf::Vector2f(x_center - _barBackgroundRectangleWidth / 2.0, _barVerticalPadding));
    
    int actualJobQuantity = actualJobQuantites[job];
    _actualJobQuantityText.setString(std::to_string(actualJobQuantity));
    _actualJobQuantityText.setPosition(sf::Vector2f(x_center + _barRectangleWidth, actual_level_y));

    int idealJobQuantity = static_cast<int>(std::round(_idealJobLevels[job] * colonySize));
    _idealJobQuantityText.setString(std::to_string(idealJobQuantity));
    _idealJobQuantityText.setPosition(sf::Vector2f(x_center - _barRectangleWidth - _idealJobQuantityText.getLocalBounds().width, ideal_level_y));

    if (idealJobQuantity < actualJobQuantity) {
      _actualJobQuantityText.setFillColor(sf::Color::Cyan);
    } else {
      _actualJobQuantityText.setFillColor(sf::Color::Red);
    }

    _jobNameText.setString(jobName);
    _jobNameText.setOrigin(_jobNameText.getLocalBounds().width / 2.0, 0.0);
    _jobNameText.setPosition(sf::Vector2f(x_center, _barBackgroundRectangle.getSize().y + _barVerticalPadding + 10.0));

    window.draw(_jobNameText);
    window.draw(_idealLevelRectangle);
    window.draw(_barBackgroundRectangle);
    window.draw(_barRectangle);
    window.draw(_idealJobQuantityText);
    window.draw(_actualJobQuantityText);
  }
}
