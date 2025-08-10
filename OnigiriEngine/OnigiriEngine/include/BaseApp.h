#pragma once

/**
 * @file BaseApp.h
 * @brief Defines the BaseApp class, which manages the main application loop and rendering.
 */

#include <Window.h>
#include "CShape.h" // Agregado para que coincida con el código del profesor
#include "ECS/Actor.h"
#include "EngineGUI.h"

#include <vector>

 /**
  * @class BaseApp
  * @brief Core application class that controls initialization, the main loop, rendering, and cleanup.
  */
class
BaseApp {
public:
  /**
   * @brief Default constructor.
   */
  BaseApp() = default;

  /**
   * @brief Destructor that handles cleanup.
   */
  ~BaseApp();

  /**
   * @brief Runs the application.
   *
   * This method initializes the application, enters the main loop, and calls update/render methods.
   * @return Exit code of the application.
   */
  int
  run();

  /**
   * @brief Initializes the application window and objects.
   * @return True if initialization was successful, false otherwise.
   */
  bool
  init();

  /**
   * @brief Updates the application logic (called every frame).
   */
  void
  update();

  /**
   * @brief Renders all drawable objects to the screen.
   */
  void
  render();

  /**
   * @brief Releases all allocated resources and cleans up.
   */
  void
  destroy();

private:
  std::vector<EngineUtilities::TSharedPointer<Actor>> m_actors;

  EngineUtilities::TSharedPointer<Window> m_windowPtr;   ///< Pointer to custom Window class using smart pointer.
  EngineUtilities::TSharedPointer<CShape> m_shapePtr;    ///< Pointer to custom shape class using smart pointer.
  EngineUtilities::TSharedPointer<Actor> m_ACircle;
  EngineUtilities::TSharedPointer<Actor> m_Track;

  std::vector<sf::Vector2f> m_waypoints; ///< Lista de posiciones a seguir por el actor.
  int m_currentWaypointIndex = 0;        ///< Índice del waypoint actual.

  std::vector<EngineUtilities::TSharedPointer<CShape>> m_waypointMarkers;

  EngineGUI m_engineGUI;

private:
  int  m_totalLaps = 3;

  // Estado del jugador (Yoshi)
  int  m_playerLapCount = 0;
  int  m_prevPlayerWaypointIndex = 0;
  bool m_playerFinished = false;

  // Meta y final
  bool m_raceFrozen = false;
  std::vector<std::string> m_finishOrder; // nombres en orden de llegada (incluye player)
  std::string m_winnerName;

  sf::Clock m_raceClock;   // Para medir el tiempo de carrera
  float m_elapsedTime = 0; // Tiempo acumulado en segundos
  bool m_raceFinished = false; // Indica si la carrera terminó

private:
  float m_playerSpeed = 200.f;

  // Reglas por waypoint del jugador (probabilidad y rango). Si queda vacío,
  // Por defecto p=1.0, min=150, max=350.
  struct PlayerSpeedRule {
    float p = 1.0f;
    float minS = 150.f;
    float maxS = 350.f;
  };
  std::vector<PlayerSpeedRule> m_playerSpeedRules;


};
