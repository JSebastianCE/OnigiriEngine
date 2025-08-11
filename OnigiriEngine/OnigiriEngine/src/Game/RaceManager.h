#pragma once
#include <vector>
#include <string>
#include <SFML/System/Clock.hpp>
#include "ECS/Actor.h"
#include "WaypointSystem.h"
#include "A_Racer.h"
#include "A_Player.h"

/**
 * @file RaceManager.h
 * @brief Declara la clase RaceManager y el tipo StandingRow para gestionar una carrera.
 */

 /**
  * @struct StandingRow
  * @brief Fila de clasificación para mostrar en HUD.
  *
  * Describe el estado de un competidor (NPC o jugador) en la tabla de posiciones.
  */
struct 
StandingRow {
  std::string name; // Nombre para mostrar en la clasificación.

  int   laps = 0; // Vueltas completadas por el competidor.
  int   wp = 0; // Índice del waypoint actual (progreso dentro de la vuelta).
  float dist = 0.f; //  Distancia al siguiente waypoint (para desempates).
  bool  isPlayer = false; // Indica si esta fila corresponde al jugador.
};

/**
 * @class RaceManager
 * @brief Orquesta la lógica de carrera: vueltas, llegadas, ranking y cronómetro.
 *
 * Se inicializa con el total de vueltas, el jugador, la lista de actores y un
 * puntero al sistema de waypoints. En cada frame, actualiza llegadas, cálculo
 * de posiciones y cronómetro; también puede congelar la carrera al llegar los
 * primeros N corredores.
 */
class 
RaceManager {
public:
  /**
   * @brief Configura el administrador de carrera.
   * @param totalLaps Número total de vueltas a completar para finalizar.
   * @param player Referencia al jugador (A_Player).
   * @param actors Lista de actores (incluye NPCs A_Racer y el propio player casteado a Actor).
   * @param wps Puntero al sistema de waypoints (para métricas si se requieren).
   */
  void 
  init(int totalLaps, const EngineUtilities::TSharedPointer<A_Player>& player,
    const std::vector<EngineUtilities::TSharedPointer<Actor>>& actors,
    WaypointSystem* wps);

  /**
  * @brief Resetea el estado de carrera y arranca el cronómetro.
  */
  void 
  start();

  /**
   * @brief Actualiza el estado de la carrera.
   * @param dt Delta de tiempo en segundos desde el último frame.
   *
   * Marca llegadas (cuando alcanzan las vueltas objetivo), recalcula el ranking
   * (laps > waypoint > distancia) y avanza el cronómetro si no está detenido.
   */
  void 
  update(float dt);  // calcula llegadas, ranking, cronómetro

  /**
   * @brief Congela la carrera cuando han llegado los primeros N.
   * @param n Número de corredores requeridos para congelar (p.ej., 3).
   *
   * Al congelar, guarda el ganador (primer elemento de m_finish) y detiene el cronómetro.
   */
  void 
  freezeIfTopNArrived(int n); // congela al llegar N corredores

  // Datos para HUD
 /**
  * @brief Devuelve la tabla de posiciones calculada en el último update.
  */
  const 
  std::vector<StandingRow>& standings() const { return m_table; }

  /**
  * @brief Indica si la carrera está congelada (finalizada para efectos de HUD).
  */
  bool 
  frozen()    
  const { return m_frozen; }


  /**
   * @brief Devuelve el nombre del ganador (primer llegado).
   */
  const 
  std::string& winner() const { return m_winner; }

  /**
   * @brief Tiempo transcurrido de carrera en segundos (cronómetro).
   */
  const 
  std::vector<std::string>& finishOrder() const { return m_finish; }

  /**
   * @brief Tiempo transcurrido de carrera en segundos (cronómetro).
   */
  float 
  elapsedSeconds() const { return m_elapsed; }

private:
  int m_totalLaps = 3; //Objetivo vueltas por finalizar
  EngineUtilities::TSharedPointer<A_Player> m_player; //Referencia al jugador
  std::vector<EngineUtilities::TSharedPointer<Actor>> m_actors; // Actores (Incluyendo NPCs)
  WaypointSystem* m_wps = nullptr; // Sistema de waypoints (si se requiere para métricas).

  // reloj
  sf::Clock m_clock; // Reloj interno para medir tiempo transcurrido.
  float m_elapsed = 0.f; // Tiempo acumulado en segundos.
  bool m_timeStop = false; // Si true, el cronómetro queda detenido.

  // meta
  bool m_frozen = false; //  Si es true, la carrera está congelada (terminada a efectos de HUD).
  std::vector<std::string> m_finish; // Orden de llegada (nombres).
  std::string m_winner; // Nombre del ganador (primer elemento de m_finish).

  // ranking
  std::vector<StandingRow> m_table; //Tabla de posiciones calculada en cada update.
};

