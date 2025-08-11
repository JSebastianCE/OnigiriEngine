#include "WaypointSystem.h"
#include <cmath>


/**
 * @file WaypointSystem.cpp
 * @brief Implementación del sistema de waypoints (detección y utilidades de debug).
 */
void 
WaypointSystem::setWaypoints(const std::vector<sf::Vector2f>& wps, float r) {
  m_wps = wps; 
  m_radius = r;
}

bool 
WaypointSystem::advanceIfReached(const sf::Vector2f& pos, 
  WaypointProgress& p, int totalLaps) {

  if (m_wps.empty()) return false;

  // Distancia al waypoint objetivo actual
  sf::Vector2f tgt = m_wps[p.current];
  float dx = tgt.x - pos.x, dy = tgt.y - pos.y;

  // Si aún está fuera del radio de detección, no avanza
  if (std::sqrt(dx * dx + dy * dy) >= m_radius) return false;

  // Avanza al siguiente waypoint (cíclico)
  p.previous = p.current;
  p.current = (p.current + 1) % (int)m_wps.size();

  // ¿Se cruzó la meta? (del último al primero)
  if (p.previous == (int)m_wps.size() - 1 && p.current == 0) {
    p.laps++;
    if (p.laps >= totalLaps) p.finished = true;
  }
  return true;
}

void 
WaypointSystem::buildMarkers(std::vector<EngineUtilities
  ::TSharedPointer<CShape>>& out) {
  out.clear();
  for (auto& wp : m_wps) {
    auto m = EngineUtilities::MakeShared<CShape>();
    m->createShape(ShapeType::CIRCLE);
    m->setFillColor(sf::Color::Yellow);
    m->setPosition(wp);
    m->setScale({ 2.f,2.f });
    out.push_back(m);
  }
}
