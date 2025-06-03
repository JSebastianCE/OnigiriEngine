#include "BaseApp.h"

/**
 * @file BaseApp.cpp
 * @brief Implements the BaseApp class which manages the main application loop.
 */

 /**
  * @brief Destructor for BaseApp.
  *
  * Currently empty, as resources are freed in the destroy() method.
  */
BaseApp::~BaseApp() {
}

/**
 * @brief Starts the main loop of the application.
 *
 * Initializes the application, enters the main loop where events are handled, and updates and renders
 * the application. Finally, it calls destroy() to release resources.
 *
 * @return int Returns 0 on successful execution.
 */
int BaseApp::run() {
  if (!init()) {
    ERROR("BaseApp", "run", "Initializes result on a false statement",
      "check method validations");
  }

  while (m_window->isOpen()) {
    m_window->handleEvents();
    update();
    render();
  }

  destroy();
  return 0;
}

/**
 * @brief Initializes the application resources.
 *
 * Creates the window and sets up a simple yellow circle shape.
 *
 * @return true if initialization succeeds.
 */
bool BaseApp::init() {
  m_window = new Window(1920, 1080, "Onigiri Engine");

  m_circle = new sf::CircleShape(100.0f);
  m_circle->setFillColor(sf::Color::Yellow);
  m_circle->setPosition(200.f, 150.f);

  return true;
}

/**
 * @brief Updates the application state.
 *
 * Empty for now. Intended for game logic or state updates.
 */
void BaseApp::update() {
}

/**
 * @brief Renders the current frame.
 *
 * Clears the screen, draws the circle shape, and displays the result.
 */
void BaseApp::render() {
  m_window->clear();
  m_window->draw(*m_circle);
  m_window->display();
}

/**
 * @brief Releases allocated resources.
 *
 * Deletes the shape and properly destroys the window.
 */
void BaseApp::destroy() {
  delete m_circle;
  m_window->destroy();
}
