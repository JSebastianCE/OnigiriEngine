#pragma once

#include "ECS/Actor.h"
#include <vector>

/**
 * @class A_Racer
 * @brief NPC corredor que sigue una ruta con comportamientos de steering.
 */
class A_Racer : public Actor {
public:
  /**
   * @brief Constructor del NPC con nombre y posición inicial.
   */
  A_Racer(const std::string& name, int initPlace);

  /**
   * @brief Actualiza la lógica del NPC cada frame.
   * @param deltaTime Tiempo transcurrido desde el último frame.
   */
  void 
  update(float deltaTime) override;

  /**
   * @brief Establece la lista de waypoints que debe seguir.
   */
  void 
  setWaypoints(const std::vector<sf::Vector2f>& waypoints);

  /**
   * @brief Reinicia el NPC a su punto inicial.
   */
  void 
  resetPosition();

  void
  setSpeed(float newSpeed);

private:
  int initialPositionIndex = 0;
  int currentWaypointIndex = 0;

  float speed = 100.0f;
  float maxSpeed = 200.0f;
  float steeringForce = 10.0f;

  std::vector<sf::Vector2f> m_waypoints;

  void 
  seekToCurrentWaypoint(float deltaTime);
};
