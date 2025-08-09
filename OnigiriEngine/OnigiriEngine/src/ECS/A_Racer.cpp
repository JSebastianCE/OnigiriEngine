#include "A_Racer.h"
#include "ECS/Transform.h"
#include <cmath>
#include <limits>

A_Racer::A_Racer(const std::string& name, int initPlace)
  : Actor(name), initialPositionIndex(initPlace), currentWaypointIndex(initPlace) {
}

void 
A_Racer::setWaypoints(const std::vector<sf::Vector2f>& waypoints) {
  m_waypoints = waypoints;
  resetPosition();
}

void 
A_Racer::resetPosition() {
  if (!m_waypoints.empty()) {
    lapCount = 0;
    finished = false;
    currentWaypointIndex = initialPositionIndex;
    prevWaypointIndex = currentWaypointIndex;
    if (auto t = getComponent<Transform>()) {
      t->setPosition(m_waypoints[currentWaypointIndex]);
    }
  }
}

void 
A_Racer::update(float deltaTime) {
  if (finished) { Actor::update(deltaTime); return; } // Mantiene el render sync, pero no muevas
  Actor::update(deltaTime);
  if (!m_waypoints.empty()) {
    seekToCurrentWaypoint(deltaTime);
  }
}

void 
A_Racer::seekToCurrentWaypoint(float deltaTime) {
  auto transform = getComponent<Transform>();
  if (!transform || m_waypoints.empty()) return;

  sf::Vector2f pos = transform->getPosition();
  sf::Vector2f target = m_waypoints[currentWaypointIndex];

  sf::Vector2f dir = target - pos;
  float distance = std::sqrt(dir.x * dir.x + dir.y * dir.y);

  // Próximo waypoint y chequeo de vuelta
  if (distance < 10.0f) {
    prevWaypointIndex = currentWaypointIndex;
    currentWaypointIndex++;
    if (currentWaypointIndex >= static_cast<int>(m_waypoints.size())) {
      currentWaypointIndex = 0;
    }
    // Si venías del último y pasas a 0 -> cruzaste meta
    if (prevWaypointIndex == static_cast<int>(m_waypoints.size()) - 1 &&
      currentWaypointIndex == 0) {
      lapCount++;
    }
    target = m_waypoints[currentWaypointIndex];
    dir = target - pos;
  }

  float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);
  if (length > 0.0f) dir /= length;

  sf::Vector2f velocity = dir * speed;
  transform->setPosition(pos + velocity * deltaTime);
}

void 
A_Racer::setSpeed(float newSpeed) { speed = newSpeed; }

float A_Racer::getDistanceToNextWaypoint() const {
  if (m_waypoints.empty()) return std::numeric_limits<float>::max();
  auto t = const_cast<A_Racer*>(this)->getComponent<Transform>();
  if (!t) return std::numeric_limits<float>::max();
  sf::Vector2f pos = t->getPosition();
  sf::Vector2f target = m_waypoints[currentWaypointIndex];
  float dx = target.x - pos.x;
  float dy = target.y - pos.y;
  return std::sqrt(dx * dx + dy * dy);
}
