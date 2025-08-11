#pragma once

#include "ECS/Actor.h"
#include <vector>

/**
 * @class A_Racer
 * @brief Representa un NPC corredor que sigue una ruta de waypoints usando un
 *        comportamiento básico de steering y con posibilidad de cambiar de velocidad
 *        según reglas predefinidas.
 *
 * Cada NPC:
 *  - Mantiene su progreso en los waypoints y vueltas.
 *  - Aplica reglas de velocidad (probabilidad y rango) al alcanzar ciertos waypoints.
 *  - Se mueve automáticamente hacia su siguiente objetivo.
 */
class A_Racer : public Actor {
public:
  /**
  * @struct SpeedRule
  * @brief Define la probabilidad y rango de velocidad que un NPC puede adoptar
  *        al pasar por un waypoint específico.
  */
  struct 
  SpeedRule {
    float p = 1.0f;       // probabilidad de cambiar velocidad (1 = siempre)
    float minS = 150.f;   // velocidad mínima
    float maxS = 350.f;   // velocidad máxima
  };

  /**
   * @brief Constructor del NPC.
   * @param name Nombre del actor.
   * @param initPlace Índice inicial de waypoint donde empieza el NPC.
   */
  A_Racer(const std::string& name, int initPlace);

  /**
 * @brief Actualiza el NPC cada frame.
 * @param deltaTime Tiempo transcurrido desde el último frame (en segundos).
 */
  void 
  update(float deltaTime) override;

  /**
   * @brief Asigna la lista de waypoints que el NPC seguirá.
   * @param waypoints Lista de posiciones 2D.
   */
  void 
  setWaypoints(const std::vector<sf::Vector2f>& waypoints);


  /**
   * @brief Restaura el NPC a su posición inicial y reinicia su estado de carrera.
   */
  void
  resetPosition();

  /**
  * @brief Configura la velocidad base del NPC.
  * @param newSpeed Nueva velocidad.
  */
  void 
  setSpeed(float newSpeed);

  /**
  * @brief Configura las reglas de velocidad para cada waypoint.
  * @param rules Vector con reglas por índice de waypoint.
  */
  void 
  setSpeedRules(const std::vector<SpeedRule>& rules);

  /**
  * @brief Obtiene el índice del waypoint actual que el NPC está siguiendo.
  */
  int   
  getCurrentWaypointIndex() const { return currentWaypointIndex; }


  /**
   * @brief Calcula la distancia al siguiente waypoint.
   */
  float 
  getDistanceToNextWaypoint() const;


  /**
   * @brief Obtiene el número de vueltas completadas.
   */
  int  
  getLap() const { return lapCount; }


  /**
   * @brief Indica si el NPC ya terminó la carrera.
   */
  bool  
  isFinished() const { return finished; }

  /**
   * @brief Marca al NPC como finalizado o no.
   * @param v true para marcar como finalizado.
   */
  void  
  markFinished(bool v) { finished = v; }


private:
  int initialPositionIndex = 0; // Índice inicial en la lista de waypoints
  int currentWaypointIndex = 0; // Índice del waypoint actual.

  int  lapCount = 0; // Vueltas completadas.
  int  prevWaypointIndex = 0; // Último waypoint alcanzado.
  bool finished = false; // Estado de carrera (finalizado o no).

  float speed = 100.0f; //Velocidad actual.
  float maxSpeed = 200.0f; // Velocidad máxima teórica
  float steeringForce = 10.0f; // Fuerza de giro (no usada extensivamente aquí).

  std::vector<sf::Vector2f> m_waypoints; // Lista de waypoints

  std::vector<SpeedRule> m_speedRules; // Reglas de velocidad


  /**
   * @brief Mueve al NPC hacia su waypoint actual y gestiona pasos y cambios de velocidad.
   * @param deltaTime Tiempo transcurrido desde el último frame.
   */
  void 
  seekToCurrentWaypoint(float deltaTime);

  /**
  * @brief Aplica la regla de velocidad correspondiente a un waypoint dado, con probabilidad.
  * @param waypointIndex Índice del waypoint.
  */
  void 
  maybeApplySpeedRuleFor(int waypointIndex); 
};
