#pragma once
#include <vector>
#include <SFML/System/Vector2.hpp>
#include "CShape.h"
#include "ECS/Actor.h"

/**
 * @file WaypointSystem.h
 * @brief Declara el sistema de waypoints para detección de progreso por puntos.
 */

 /**
  * @struct WaypointProgress
  * @brief Estado de progreso de un actor a través de los waypoints.
  *
  * Guarda el waypoint actual y el anterior, así como las vueltas completadas
  * y si ya terminó según el número total de vueltas de la carrera.
  */
struct 
WaypointProgress {
  int  current = 0; // Índice del waypoint objetivo actual.
  int  previous = 0; // Índice del waypoint anterior alcanzado.
  int  laps = 0; // Vueltas completadas (incrementa al pasar del último al primero).
  bool finished = false; // Indica si ya alcanzó el total de vueltas requerido.
};


/**
 * @class WaypointSystem
 * @brief Gestiona una lista ordenada de waypoints y su detección por radio.
 *
 * Permite configurar la ruta (lista de puntos 2D), definir un radio de detección
 * y avanzar el progreso de un actor cuando entra en el radio del siguiente waypoint.
 * También puede generar marcadores visuales de depuración.
 */
class 
WaypointSystem {
public:

  /**
  * @brief Configura la lista de waypoints y el radio de detección.
  * @param wps Lista ordenada de posiciones (ruta del circuito).
  * @param radiusPx Radio de detección en píxeles para considerar un waypoint alcanzado.
  */
  void 
  setWaypoints(const std::vector<sf::Vector2f>& wps, float radiusPx = 12.f);

  /**
  * @brief Acceso de solo lectura a la ruta configurada.
  * @return Referencia constante al vector de waypoints.
  */
  const std::vector<sf::Vector2f>& waypoints() const { return m_wps; }

  /**
   * @brief Devuelve el radio de detección actual.
   * @return Radio en píxeles.
   */
  float 
  radius() const { return m_radius; }

  /**
   * @brief Avanza el progreso si la posición entra en el radio del siguiente waypoint.
   * @param pos Posición actual del actor.
   * @param prog Estado de progreso del actor (se actualiza si avanza).
   * @param totalLaps Total de vueltas a completar; marca @c finished cuando se cumple.
   * @return @c true si se avanzó de waypoint (y se evaluó posible incremento de vuelta).
   *
   * La función:
   * - Comprueba si @p pos está dentro del radio del waypoint objetivo @c prog.current.
   * - Si lo está, incrementa @c prog.current (cíclico) y actualiza @c prog.previous.
   * - Si el avance fue del último al primero, incrementa @c prog.laps y,
   *   si alcanza @p totalLaps, marca @c prog.finished = true.
   */
  bool
  advanceIfReached(const sf::Vector2f& pos, WaypointProgress& prog, int totalLaps);

  /**
   * @brief Construye marcadores visuales (círculos amarillos) para depurar la ruta.
   * @param outMarkers Vector de salida donde se almacenan los marcadores creados.
   *
   * Los marcadores son de tipo @c CShape (ShapeType::CIRCLE), escalados para ser visibles.
   */
  void
  buildMarkers(std::vector<EngineUtilities::TSharedPointer<CShape>>& outMarkers);

private:
  std::vector<sf::Vector2f> m_wps; // Ruta (lista ordenada de puntos 2D).

  float m_radius = 12.f; // Radio de detección para considerar un waypoint alcanzado.
};
};
