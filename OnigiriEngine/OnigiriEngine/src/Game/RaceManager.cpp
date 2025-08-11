#include "RaceManager.h"
#include <algorithm>
#include <cmath>

/**
 * @file RaceManager.cpp
 * @brief Implementación de RaceManager: inicialización, actualización y congelación.
 */

 /**
  * @brief Distancia euclídea entre dos puntos 2D.
  * @param a Punto A.
  * @param b Punto B.
  * @return Distancia en unidades del mundo.
  */
static float 
distTo(const sf::Vector2f& a, const sf::Vector2f& b) {
  float dx = a.x - b.x, dy = a.y - b.y; return std::sqrt(dx * dx + dy * dy);
}

/**
 * @brief Configura la carrera con vueltas, jugador, actores y sistema de waypoints.
 * @param laps Vueltas objetivo.
 * @param player Referencia al jugador (A_Player).
 * @param actors Lista de actores (NPCs y player como Actor).
 * @param wps Puntero al sistema de waypoints.
 */
void 
RaceManager::init(int laps,
  const EngineUtilities::TSharedPointer<A_Player>& player,
  const std::vector<EngineUtilities::TSharedPointer<Actor>>& actors,
  WaypointSystem* wps) {
  m_totalLaps = laps;
  m_player = player;
  m_actors = actors;
  m_wps = wps;
}

/**
 * @brief Pone el estado de carrera en condiciones de inicio y reinicia el cronómetro.
 *
 * Limpia el orden de llegada, desactiva el modo congelado, reinicia tiempo y
 * arranca el reloj interno.
 */
void 
RaceManager::start() {
  m_finish.clear(); m_winner.clear();
  m_frozen = false; m_timeStop = false;
  m_elapsed = 0.f;  m_clock.restart();
}


/**
 * @brief Lógica principal por frame: cronómetro, llegadas y ranking.
 * @param dt Delta de tiempo (no es estrictamente usado aquí, pero útil si se extiende).
 *
 * - Avanza el cronómetro si no está detenido.
 * - Marca como finalizados a NPCs y jugador cuando cumplen m_totalLaps.
 * - Rellena y ordena la tabla de posiciones por (laps > waypoint > distancia).
 */
void 
RaceManager::update(float dt) {
  if (!m_timeStop) m_elapsed = m_clock.getElapsedTime().asSeconds();

  // Marcar llegadas de NPCs
  for (auto& a : m_actors) {
    if (auto r = a.dynamic_pointer_cast<A_Racer>()) {
      if (!r->isFinished() && r->getLap() >= m_totalLaps) {
        r->markFinished(true);
        r->setSpeed(0.f);
        m_finish.push_back(r->getName());
      }
    }
  }
  // Marcar llegada del player
  if (m_player && !m_player->isFinished() && 
    m_player->getLap() >= m_totalLaps) {
    m_player->markFinished(true);
    m_finish.push_back(m_player->getName());
  }

  // Ranking
  m_table.clear();
  // NPCs
  for (auto& a : m_actors) {
    if (auto r = a.dynamic_pointer_cast<A_Racer>()) {
      m_table.push_back({ r->getName(), r->getLap(),
        r->getCurrentWaypointIndex(), r->getDistanceToNextWaypoint(), false });
    }
  }
  // Player
  if (m_player) {
    m_table.push_back({ m_player->getName(), 
      m_player->getLap(),
      m_player->getCurrentWaypointIndex(), 
      m_player->getDistanceToNextWaypoint(), true });
  }

  std::sort(m_table.begin(), m_table.end(), 
    [](const StandingRow& A, const StandingRow& B) {
    if (A.laps != B.laps) return A.laps > B.laps;
    if (A.wp != B.wp)   return A.wp > B.wp;
    return A.dist < B.dist;
    });
}

/**
 * @brief Congela la carrera cuando han llegado al menos @p n corredores.
 * @param n Número de llegados a partir del cual se congela.
 *
 * Al congelar:
 * - Se fija @c m_frozen = true.
 * - Se registra el ganador en @c m_winner (primer elemento de @c m_finish).
 * - Se detiene el cronómetro ( @c m_timeStop = true ).
 */
void 
RaceManager::freezeIfTopNArrived(int n) {
  if (!m_frozen && (int)m_finish.size() >= n) {
    m_frozen = true;
    m_winner = m_finish.front();
    m_timeStop = true; // detiene cronómetro
  }
}
