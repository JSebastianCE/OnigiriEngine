#include "A_Racer.h"
#include "ECS/Transform.h"

A_Racer::A_Racer(const std::string& name, int initPlace)
  : Actor(name), initialPositionIndex(initPlace), currentWaypointIndex(initPlace) {
  // Puedes ajustar la posición inicial aquí si hay una lista de waypoints
}

void 
A_Racer::setWaypoints(const std::vector<sf::Vector2f>& waypoints) {
  m_waypoints = waypoints;
  resetPosition();
}

void 
A_Racer::resetPosition() {
  if (!m_waypoints.empty()) {
    currentWaypointIndex = initialPositionIndex;
    getComponent<Transform>()->setPosition(m_waypoints[currentWaypointIndex]);
  }
}

void 
A_Racer::update(float deltaTime) {
  Actor::update(deltaTime); // Llama a la lógica base (actualiza el shape)

  if (!m_waypoints.empty()) {
    seekToCurrentWaypoint(deltaTime);
  }
}

void 
A_Racer::seekToCurrentWaypoint(float deltaTime) {
  auto transform = getComponent<Transform>();
  if (!transform) return;

  sf::Vector2f pos = transform->getPosition();
  sf::Vector2f target = m_waypoints[currentWaypointIndex];

  sf::Vector2f direction = target - pos;
  float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

  if (distance < 10.0f) {
    currentWaypointIndex++;
    if (currentWaypointIndex >= static_cast<int>(m_waypoints.size())) {
      currentWaypointIndex = 0;
    }
    target = m_waypoints[currentWaypointIndex];
    direction = target - pos;
  }

  // Normalizar dirección
  float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
  if (length > 0.0f) {
    direction /= length;
  }

  // Aplicar steering simple
  sf::Vector2f velocity = direction * speed;
  transform->setPosition(pos + velocity * deltaTime);
}

void
A_Racer::setSpeed(float newSpeed) {
  speed = newSpeed;
}