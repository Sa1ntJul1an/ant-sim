#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "antsim.h"
#include "jobs.h"
#include "job_levels_display.h"

using namespace sf;
using namespace std;

// width in height
const int WIDTH = 1900;
const int HEIGHT = 1000;

const int JOBBARWINDOW_HEIGHT = 600;
const int JOBBARWINDOW_WIDTH = 400;

const float ANT_INTERACTION_DISTANCE = 3.0;
const int ANT_ENCOUNTER_BUFFER_SIZE = 50;

const float GRID_CELL_SPACING = ANT_INTERACTION_DISTANCE;

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
  antSim.randomColony(10000);

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
    

  // TODO: progress rectangles for each job that fill up to the ideal proportion 

  return 0;
}
