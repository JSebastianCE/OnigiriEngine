#include "BaseApp.h"
#include <ECS/Actor.h>

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
int
BaseApp::run() {
  if (!init()) {
    ERROR("BaseApp", "run", "Initializes result on a false statement",
      "check method validations");
  }

  while (m_windowPtr->isOpen()) {
    m_windowPtr->handleEvents();
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
bool
BaseApp::init() {
  m_windowPtr = EngineUtilities::MakeShared<Window>(1920, 1080, "Onigiri Engine");
  if (!m_windowPtr) {
    ERROR("BaseApp", "init", "Failed to create window pointer, check memory allocation");
    return false;
  }

  m_shapePtr = EngineUtilities::MakeShared<CShape>();
  if (m_shapePtr) {
    m_shapePtr->createShape(ShapeType::CIRCLE);
    m_shapePtr->setFillColor(sf::Color::Yellow);
    m_shapePtr->setPosition(200.f, 150.f);
  }

  /*m_shapePtr = EngineUtilities::MakeShared<CShape>();
  if (m_shapePtr) {
    m_shapePtr->createShape(ShapeType::CIRCLE);
    m_shapePtr->setFillColor(sf::Color::Blue);
    m_shapePtr->setPosition(150.f, 200.f);
  }*/


  //Create Circle Actor
  m_ACircle = EngineUtilities::MakeShared<Actor>("Circle Actor");
  if (m_ACircle) {
    m_ACircle->getComponent<CShape>()->createShape(CIRCLE);
    m_ACircle->getComponent<CShape>()->setFillColor(sf::Color::Red);
    m_ACircle->getComponent<Transform>()->setPosition(sf::Vector2 (100.f, 150.f));
    m_ACircle->setTexture();
    //m_ACircle->setName("Circle Actor");

    //Waypoints
    m_waypoints.push_back(sf::Vector2f(400.f, 150.f));
    m_waypoints.push_back(sf::Vector2f(700.f, 300.f));
    m_waypoints.push_back(sf::Vector2f(1000.f, 150.f));
    m_waypoints.push_back(sf::Vector2f(1200.f, 500.f));
  }

  return true;
}

/**
 * @brief Updates the application state.
 *
 * Empty for now. Intended for game logic or state updates.
 */
void
BaseApp::update() {
  if (!m_windowPtr.isNull()) {
    m_windowPtr->update();
  }

  if (!m_ACircle.isNull() && !m_waypoints.empty()) {
    m_ACircle->update(m_windowPtr->deltaTime.asSeconds());

    // Obtener el destino actual del waypoint
    sf::Vector2f targetPos = m_waypoints[m_currentWaypointIndex];

    // Obtener posición actual del actor
    sf::Vector2f currentPos = m_ACircle->getComponent<Transform>()->getPosition();

    // Calcular distancia al waypoint
    float dx = targetPos.x - currentPos.x;
    float dy = targetPos.y - currentPos.y;
    float distance = std::sqrt(dx * dx + dy * dy);

    // Si llegó al waypoint actual, pasar al siguiente
    if (distance < 10.0f) {
      m_currentWaypointIndex++;
      if (m_currentWaypointIndex >= static_cast<int>(m_waypoints.size())) {
        m_currentWaypointIndex = 0; // Volver al inicio (comportamiento cíclico)
      }
    }

    // Buscar hacia el waypoint actual
    m_ACircle->getComponent<Transform>()->seek(
      m_waypoints[m_currentWaypointIndex],
      200.0f,
      m_windowPtr->deltaTime.asSeconds(),
      10.0f
    );
  }
}


/**
 * @brief Renders the current frame.
 *
 * Clears the screen, draws the circle shape, and displays the result.
 */
void
BaseApp::render() {
  if (!m_windowPtr) {
    return;
  }

  m_windowPtr->clear();

  if (m_shapePtr) {
    m_shapePtr->render(m_windowPtr);
  }

  if (!m_ACircle.isNull()) {
    m_ACircle->render(m_windowPtr);
  }

  m_windowPtr->display();
}


/**
 * @brief Releases allocated resources.
 *
 * Deletes the shape and properly destroys the window.
 */
void
BaseApp::destroy() {
  // m_shapePtr.Reset(); // Not necessary if using smart pointers correctly
  // m_windowPtr.Reset(); // Cleanup handled automatically
}
