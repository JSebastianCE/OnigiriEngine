#include "A_Player.h"
#include "ECS/Transform.h"
#include <SFML/Window/Keyboard.hpp>
#include <cmath>
#include <random>

// RNG local al archivo
static std::mt19937& rng_player() {
  static std::mt19937 eng{ std::random_device{}() };
  return eng;
}

A_Player::A_Player(const std::string& name)
  : Actor(name) {
}

void A_Player::setWaypoints(const std::vector<sf::Vector2f>& wps) {
  m_waypoints = wps;
  if (m_speedRules.size() != m_waypoints.size()) {
    m_speedRules.assign(m_waypoints.size(), SpeedRule{ 1.0f, 150.f, 350.f });
  }
  // Reset básico
  m_lapCount = 0;
  m_finished = false;
  m_currentWp = 0;
  m_prevWp = m_currentWp;
  m_speed = m_baseSpeed;

  if (auto t = getComponent<Transform>()) {
    if (!m_waypoints.empty())
      t->setPosition(m_waypoints[m_currentWp]);
  }
}

void A_Player::setSpeedRules(const std::vector<SpeedRule>& rules) {
  m_speedRules = rules;
  if (m_waypoints.size() && m_speedRules.size() != m_waypoints.size()) {
    m_speedRules.resize(m_waypoints.size(), SpeedRule{ 1.0f, 150.f, 350.f });
  }
}

void A_Player::setBaseSpeed(float s) {
  m_baseSpeed = s;
  m_speed = s;
}

sf::Vector2f A_Player::getDirFromKeys() const {
  float x = 0.f, y = 0.f;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) y -= 1.f;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) y += 1.f;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) x -= 1.f;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) x += 1.f;

  sf::Vector2f v{ x, y };
  float len = std::sqrt(v.x * v.x + v.y * v.y);
  if (len > 0.f) {
    v.x /= len;
    v.y /= len;
  }
  return v; // (0,0) si no hay teclas
}

void A_Player::update(float deltaTime) {
  Actor::update(deltaTime); // sincroniza Transform->Shape

  if (m_finished || m_waypoints.empty()) return;

  handleInput(deltaTime);

  // Checar cruce de waypoint
  auto t = getComponent<Transform>();
  if (!t) return;

  sf::Vector2f pos = t->getPosition();
  sf::Vector2f target = m_waypoints[m_currentWp];
  float dx = target.x - pos.x;
  float dy = target.y - pos.y;
  float dist = std::sqrt(dx * dx + dy * dy);

  if (dist < m_wpRadius) {
    onReachNextWaypoint();
  }
}

void A_Player::handleInput(float dt) {
  auto t = getComponent<Transform>();
  if (!t) return;

  sf::Vector2f dir = getDirFromKeys();
  if (dir.x == 0.f && dir.y == 0.f) {
    // Si no presionas nada, no avanzas.
    return;
  }

  // Movimiento simple (sin aceleraciones)
  sf::Vector2f pos = t->getPosition();
  pos += dir * m_speed * dt;
  t->setPosition(pos);
}

void A_Player::onReachNextWaypoint() {
  m_prevWp = m_currentWp;
  m_currentWp++;
  if (m_currentWp >= static_cast<int>(m_waypoints.size())) {
    m_currentWp = 0;
  }

  // cruce de meta: de último -> 0
  if (m_prevWp == static_cast<int>(m_waypoints.size()) - 1 && m_currentWp == 0) {
    m_lapCount++;
  }

  // cambiar velocidad con probabilidad
  maybeApplySpeedRuleFor(m_currentWp);
}

void A_Player::maybeApplySpeedRuleFor(int wpIndex) {
  if (m_speedRules.empty()) return;
  const auto& r = m_speedRules[wpIndex];

  std::uniform_real_distribution<float> coin(0.f, 1.f);
  if (coin(rng_player()) <= r.p) {
    float lo = std::min(r.minS, r.maxS);
    float hi = std::max(r.minS, r.maxS);
    std::uniform_real_distribution<float> pick(lo, hi);
    m_speed = pick(rng_player());
  }
}

float A_Player::getDistanceToNextWaypoint() const {
  auto t = const_cast<A_Player*>(this)->getComponent<Transform>();
  if (!t || m_waypoints.empty()) return 1e9f;
  sf::Vector2f pos = t->getPosition();
  sf::Vector2f target = m_waypoints[m_currentWp];
  float dx = target.x - pos.x;
  float dy = target.y - pos.y;
  return std::sqrt(dx * dx + dy * dy);
}
