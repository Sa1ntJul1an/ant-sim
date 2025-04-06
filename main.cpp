#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <random>

#include "antsim.h"
#include "jobs.h"
#include "job_levels_display.h"

using namespace sf;
using namespace std;

// width in height
const int WIDTH = 1900;
const int HEIGHT = 1000;

const int JOBBARWINDOW_HEIGHT = 600;
const int JOBBARWINDOW_WIDTH = 500;

const float ANT_INTERACTION_DISTANCE = 3.0;
const int ANT_ENCOUNTER_BUFFER_SIZE = 30;

const float GRID_CELL_SPACING = ANT_INTERACTION_DISTANCE;

const float ANT_SPAWNING_RADIUS = 35.0;


pair<float, float> polarToCartesian(float radius, float angleDeg) {
  float angleRad = angleDeg * M_PI / 180.0;
  
  float x = radius * cos(angleRad);
  float y = radius * sin(angleRad);

  return {x, y};
}


int main(){

  Vector2i mousePosition;

  // RENDER WINDOWS
  // =========================================================
  RenderWindow renderWindow(VideoMode(WIDTH, HEIGHT), "Ant Sim");
  renderWindow.setFramerateLimit(60);
  
  RenderWindow jobBarWindow(VideoMode(JOBBARWINDOW_WIDTH, JOBBARWINDOW_HEIGHT), "Job Levels");
  jobBarWindow.setFramerateLimit(60);
  // =========================================================

  Event renderWindowEvent;
  Event jobBarWindowEvent;

  Font font;
  FileInputStream fontIn;
  fontIn.open("slkscr.ttf");
  font.loadFromStream(fontIn);

  Text iterationText;
  iterationText.setFillColor(Color::White);
  iterationText.setPosition(10, 10);
  iterationText.setFont(font);
  iterationText.setCharacterSize(30);

  int iteration = 0;
  
  bool mouse_held = false;
  bool spacebar_held = false;

  bool sim_running = false;

  const map<Job, Color> jobColors = {
    {Job::Guard, Color::Red},
    {Job::Scout, Color::Yellow},
    {Job::Tunneler, Color::Cyan},
    {Job::Gatherer, Color::Green},
    {Job::Caretaker, Color::Magenta}
  };

  const map<Job, float> ideaJobProportions = {
    {Job::Gatherer, 0.4},
    {Job::Tunneler, 0.1},
    {Job::Scout, 0.05},
    {Job::Caretaker, 0.25},
    {Job::Guard, 0.2}
  };

  pair<int, int> dims = {WIDTH, HEIGHT};
  AntSim antSim = AntSim(ideaJobProportions, ANT_ENCOUNTER_BUFFER_SIZE, ANT_INTERACTION_DISTANCE, GRID_CELL_SPACING, jobColors, dims);
  /*antSim.randomColony(10000);*/

  std::random_device rd;
  std::mt19937 gen(rd());

  std::uniform_real_distribution<float> angleDist(0.0f, 360.0f);
  std::uniform_real_distribution<float> radiusDist(0.0f, ANT_SPAWNING_RADIUS);

  Job antJob = Job::Guard;

  int colony_size = antSim.getColonySize();

  JobLevelsDisplay jobLevelsDisplay = JobLevelsDisplay(colony_size, ideaJobProportions, jobColors, JOBBARWINDOW_WIDTH, JOBBARWINDOW_HEIGHT, font);

  while(renderWindow.isOpen()) {
    renderWindow.clear();
    jobBarWindow.clear(Color(30, 30, 30));

    mousePosition = Mouse::getPosition(renderWindow);

    // Ant Sim =================================================
    if (sim_running) {
      antSim.update(); 

      iteration ++;
      iterationText.setString("Iteration: " + to_string(iteration));
    }
    
    antSim.drawSim(renderWindow);
    // =========================================================

    jobLevelsDisplay.drawDisplay(jobBarWindow, antSim.getColonySize(), antSim.getActualJobQuantities()); 

    // MOUSE INTERACTION ======================================
    if (Mouse::isButtonPressed(Mouse::Left)) {
      if (!sim_running) {
        // spawn ants 
        float angleDeg = angleDist(gen);
        float radius = radiusDist(gen);

        std::pair<float, float> cartesianCoords = polarToCartesian(radius, angleDeg);
        
        Ant* ant = new Ant(antJob, ANT_ENCOUNTER_BUFFER_SIZE, ideaJobProportions);
        ant->setJob(antJob);
        ant->setPosition({mousePosition.x + cartesianCoords.first, mousePosition.y + cartesianCoords.second});
      
        antSim.addAnt(ant);
      }
    }
    // ========================================================

    // KEYBOARD EVENTS =========================================
    if (Keyboard::isKeyPressed(Keyboard::Space) && !spacebar_held){   // space to pause / unpause
      sim_running = !sim_running;
      spacebar_held = true;
    } else if (!Keyboard::isKeyPressed(Keyboard::Space)) {
      spacebar_held = false;
    }
    // =========================================================


    // CLOSE WINDOWS IF X PRESSED
    // =========================================================
    while(renderWindow.pollEvent(renderWindowEvent)) {
      if(renderWindowEvent.type == Event::Closed) {
        renderWindow.close();
        jobBarWindow.close();
      }
    }
    while(jobBarWindow.pollEvent(jobBarWindowEvent)) {
      if (jobBarWindowEvent.type == Event::Closed) {
        renderWindow.close();
        jobBarWindow.close();
      }
    }
    // =========================================================

    renderWindow.draw(iterationText);

    renderWindow.display();
    jobBarWindow.display();
  }
  return 0;
}
