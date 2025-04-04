#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "antsim.h"
#include "jobs.h"

using namespace sf;
using namespace std;

// width in height, in grid cells
const int WIDTH = 1900;
const int HEIGHT = 1000;

int main(){

  Vector2i mousePosition;

  // RENDER WINDOW
  // =========================================================
  RenderWindow renderWindow(VideoMode(WIDTH, HEIGHT), "Ant Sim");
  renderWindow.setFramerateLimit(60);
  // =========================================================

  Event renderWindowEvent;

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
  AntSim antSim = AntSim(ideaJobProportions, 50, jobColors, dims);
  antSim.randomColony(10000);

  while(renderWindow.isOpen()){
    renderWindow.clear();

    mousePosition = Mouse::getPosition(renderWindow);

    // Ant Sim =================================================
    if (sim_running) {
      antSim.update(); 

      iteration ++;
      iterationText.setString("Iteration: " + to_string(iteration));
    }
    
    antSim.drawSim(renderWindow);
    // =========================================================

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
    while(renderWindow.pollEvent(renderWindowEvent)){
      if(renderWindowEvent.type == Event::Closed){
        renderWindow.close();
      }
    }
    // =========================================================

    renderWindow.draw(iterationText);

    renderWindow.display();
  }
    

  // TODO: progress rectangles for each job that fill up to the ideal proportion 

  return 0;
}
