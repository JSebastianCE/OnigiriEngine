#pragma once
#include "ECS/Actor.h"
#include <vector>
#include <string>
/**
 * @class A_Player
 * @brief Actor controlado por el jugador.
 *
 * Permite el movimiento con las teclas WASD, controla el paso por waypoints,
 * cuenta las vueltas y gestiona cambios de velocidad según reglas de probabilidad.
 */
class A_Player : public Actor {
public:


  /**
   * @struct SpeedRule
   * @brief Regla de velocidad para un waypoint específico.
   *
   * Define la probabilidad y el rango de velocidad al pasar por un waypoint.
   */
struct 
SpeedRule {
    float p = 1.0f;   // probabilidad de cambiar velocidad al llegar al waypoint
    float minS = 150.f;  // velocidad mínima si hay cambio
    float maxS = 350.f;  // velocidad máxima si hay cambio
  };

   /**
   * @brief Constructor del jugador.
   * @param name Nombre identificador del actor.
   */
  explicit 
  A_Player(const std::string& name);

  /**
   * @brief Actualiza la lógica del jugador.
   *
   * Procesa entrada de teclado, movimiento, detección de waypoints
   * y cuenta de vueltas.
   *
   * @param deltaTime Tiempo transcurrido desde el último frame (en segundos).
   */
  void 
  update(float deltaTime) override;

  /**
  * @brief Asigna los waypoints que debe seguir el jugador.
  *
  * Reinicia el estado de carrera y posiciona al jugador
  * en el primer waypoint.
  *
  * @param wps Vector de posiciones 2D de los waypoints.
  */
  void 
  setWaypoints(const std::vector<sf::Vector2f>& wps);

  /**
  * @brief Asigna las reglas de velocidad para cada waypoint.
  *
  * Ajusta el tamaño del vector de reglas si no coincide
  * con el número de waypoints.
  *
  * @param rules Vector con reglas de velocidad.
  */
  void 
  setSpeedRules(const std::vector<SpeedRule>& rules);

  /**
  * @brief Establece la velocidad base del jugador.
  *
  * Esta velocidad se usará al iniciar la carrera
  * y como referencia antes de aplicar reglas.
  *
  * @param s Velocidad inicial.
  */
  void 
  setBaseSpeed(float s); // velocidad inicial

  /**
   * @brief Obtiene la velocidad actual del jugador.
   * @return Velocidad actual.
   */
  float 
  getCurrentSpeed() const { return m_speed; }

  /**
  * @brief Define el radio de detección de un waypoint.
  *
  * Si el jugador no detecta waypoints, este valor puede aumentarse.
  *
  * @param r Radio en píxeles.
  */
  void 
  setWaypointRadius(float r) { m_wpRadius = r; }

  /**
   * @brief Obtiene el radio de detección de waypoints.
   * @return Radio en píxeles.
   */
  float 
  getWaypointRadius() const { return m_wpRadius; }

  /**
  * @brief Obtiene el número de vueltas completadas.
  * @return Vueltas completadas.
  */
  int   
  getLap() const { return m_lapCount; }

   /**
   * @brief Obtiene el índice del waypoint actual.
   * @return Índice del waypoint.
   */
  int  
  getCurrentWaypointIndex() const { return m_currentWp; }


  /**
   * @brief Calcula la distancia al próximo waypoint.
   * @return Distancia en píxeles.
   */
  float 
  getDistanceToNextWaypoint() const;

  /**
   * @brief Verifica si el jugador ha terminado la carrera.
   * @return true si terminó, false en caso contrario.
   */
  bool 
  isFinished() const { return m_finished; }

  /**
  * @brief Marca el estado de finalización del jugador.
  * @param v true para marcar como terminado, false para reiniciar.
  */
  void 
  markFinished(bool v) { m_finished = v; }

private:
  /**
   * @brief Procesa la entrada de teclado y mueve al jugador.
   * @param dt Tiempo transcurrido desde el último frame.
   */
  void 
  handleInput(float dt);

  /**
   * @brief Llamado cuando el jugador alcanza un waypoint.
   *
   * Avanza al siguiente waypoint y verifica cruce de meta.
   */
  void 
  onReachNextWaypoint();  // llamado cuando se cruza un waypoint


  /**
   * @brief Aplica reglas de velocidad según el waypoint.
   * @param wpIndex Índice del waypoint actual.
   */
  void 
  maybeApplySpeedRuleFor(int wpIndex);  // cambio de velocidad con prob.

  
  std::vector<sf::Vector2f> m_waypoints; // Lista de posiciones de waypoints
  std::vector<SpeedRule> m_speedRules; /// Reglas de velocidad asociadas a cada waypoint.

  int m_currentWp = 0; //Índice del waypoint actual.
  int m_prevWp = 0; // Índice del waypoint previo.
  int m_lapCount = 0; // Número de vueltas completadas.
  bool m_finished = false; // Estado de finalización.

  // Movimiento
  float m_speed = 200.f;   // velocidad actual (cambia por rules)
  float m_baseSpeed = 200.f;   // velocidad base al iniciar carrera
  float m_wpRadius = 40.f;    // radio de detección de waypoint

  /**
   * @brief Calcula la dirección de movimiento en base a teclas presionadas.
   * @return Vector de dirección normalizado (0,0 si no hay entrada).
   */
  sf::Vector2f getDirFromKeys() const;         // dirección WASD normalizada
};
