#pragma once
#include "ECS/Actor.h"
#include <vector>
#include <string>

/**
 * @class A_Player
 * @brief Actor controlado por el jugador. Se mueve con WASD y lleva control de waypoints y vueltas.
 */
class A_Player : public Actor {
public:
  struct SpeedRule {
    float p = 1.0f;   // probabilidad de cambiar velocidad al llegar al waypoint
    float minS = 150.f;  // velocidad mínima si hay cambio
    float maxS = 350.f;  // velocidad máxima si hay cambio
  };

  explicit A_Player(const std::string& name);

  void update(float deltaTime) override;

  // Waypoints & reglas
  void setWaypoints(const std::vector<sf::Vector2f>& wps);
  void setSpeedRules(const std::vector<SpeedRule>& rules);

  // Velocidades
  void setBaseSpeed(float s);            // velocidad inicial
  float getCurrentSpeed() const { return m_speed; }

  // Radio de detección del waypoint (si te cuesta detectarlos, súbelo)
  void setWaypointRadius(float r) { m_wpRadius = r; }
  float getWaypointRadius() const { return m_wpRadius; }

  // Progreso en carrera
  int   getLap() const { return m_lapCount; }
  int   getCurrentWaypointIndex() const { return m_currentWp; }
  float getDistanceToNextWaypoint() const;
  bool  isFinished() const { return m_finished; }
  void  markFinished(bool v) { m_finished = v; }

private:
  // Entrada de teclado y movimiento
  void handleInput(float dt);
  void onReachNextWaypoint();                  // llamado cuando se cruza un waypoint
  void maybeApplySpeedRuleFor(int wpIndex);    // cambio de velocidad con prob.

  // Estado de carrera
  std::vector<sf::Vector2f> m_waypoints;
  std::vector<SpeedRule>    m_speedRules;

  int   m_currentWp = 0;
  int   m_prevWp = 0;
  int   m_lapCount = 0;
  bool  m_finished = false;

  // Movimiento
  float m_speed = 200.f;   // velocidad actual (cambia por rules)
  float m_baseSpeed = 200.f;   // velocidad base al iniciar carrera
  float m_wpRadius = 14.f;    // radio de detección de waypoint

  // Utilidad
  sf::Vector2f getDirFromKeys() const;         // dirección WASD normalizada
};
