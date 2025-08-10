#include "A_Racer.h"
#include "ECS/Transform.h"
#include <cmath>
#include <limits>
#include <random>  

// RNG global al módulo (se inicializa una sola vez)
static 
std::mt19937& rng() {
  static std::mt19937 eng{ std::random_device{}() };
  return eng;
}

A_Racer::A_Racer(const std::string& name, int initPlace)
  : Actor(name), initialPositionIndex(initPlace), currentWaypointIndex(initPlace) {
}

void 
A_Racer::setWaypoints(const std::vector<sf::Vector2f>& waypoints) {
  m_waypoints = waypoints;
  // si no hay reglas, crea por defecto (siempre cambia entre 150-350)
  if (m_speedRules.size() != m_waypoints.size()) {
    m_speedRules.assign(m_waypoints.size(), SpeedRule{ 1.0f, 150.f, 350.f });
  }
  resetPosition();
}

void 
A_Racer::setSpeedRules(const std::vector<SpeedRule>& rules) {
  m_speedRules = rules;
  // asegura tamaño correcto si difiere
  if (m_speedRules.size() != m_waypoints.size() && !m_waypoints.empty()) {
    m_speedRules.resize(m_waypoints.size(), SpeedRule{ 1.0f, 150.f, 350.f });
  }
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
    // aplica una velocidad inicial coherente con el primer waypoint
    maybeApplySpeedRuleFor(currentWaypointIndex);
  }
}

void 
A_Racer::update(float deltaTime) {
  if (finished) { Actor::update(deltaTime); return; }

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

  // llegó al waypoint
  if (distance < 10.0f) {
    prevWaypointIndex = currentWaypointIndex;
    currentWaypointIndex++;
    if (currentWaypointIndex >= static_cast<int>(m_waypoints.size())) {
      currentWaypointIndex = 0;
    }
    // cruce de meta
    if (prevWaypointIndex == static_cast<int>(m_waypoints.size()) - 1 &&
      currentWaypointIndex == 0) {
      lapCount++;
    }

    // Tal vez cambiar velocidad al entrar al nuevo waypoint
    maybeApplySpeedRuleFor(currentWaypointIndex);

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

float 
A_Racer::getDistanceToNextWaypoint() const {
  if (m_waypoints.empty()) return std::numeric_limits<float>::max();

  auto t = const_cast<A_Racer*>(this)->getComponent<Transform>();

  if (!t) return std::numeric_limits<float>::max();

  sf::Vector2f pos = t->getPosition();

  sf::Vector2f target = m_waypoints[currentWaypointIndex];
  float dx = target.x - pos.x;
  float dy = target.y - pos.y;
  return std::sqrt(dx * dx + dy * dy);
}

// NUEVO: aplica probabilidad y rango por waypoint
void 
A_Racer::maybeApplySpeedRuleFor(int waypointIndex) {
  if (m_speedRules.empty()) return;

  const auto& rule = m_speedRules[waypointIndex];

  std::uniform_real_distribution<float> coin(0.0f, 1.0f);

  if (coin(rng()) <= rule.p) {
    float lo = std::min(rule.minS, rule.maxS);
    float hi = std::max(rule.minS, rule.maxS);

    std::uniform_real_distribution<float> pick(lo, hi);

    speed = pick(rng());
  }
}
