#include "A_Racer.h"
#include "ECS/Transform.h"
#include <cmath>
#include <limits>
#include <random>  

/**
 * @brief Generador de números aleatorios global para el módulo.
 * Se inicializa una sola vez y se reutiliza en cada llamada.
 * @return Referencia a un motor Mersenne Twister.
 */
static
std::mt19937& rng() {
  static std::mt19937 eng{ std::random_device{}() };
  return eng;
}

/**
 * @brief Constructor del NPC corredor.
 * @param name Nombre identificador del actor.
 * @param initPlace Índice inicial del waypoint donde comenzará.
 */
A_Racer::A_Racer(const std::string& name, int initPlace)
  : Actor(name), initialPositionIndex(initPlace), currentWaypointIndex(initPlace) {
}

/**
 * @brief Asigna la lista de waypoints que seguirá el NPC.
 * También inicializa las reglas de velocidad por defecto si estas no
 * coinciden con el número de waypoints.
 * @param waypoints Vector de coordenadas 2D que forman la ruta.
 */
void 
A_Racer::setWaypoints(const std::vector<sf::Vector2f>& waypoints) {
  m_waypoints = waypoints;
  // si no hay reglas, crea por defecto (siempre cambia entre 150-350)
  if (m_speedRules.size() != m_waypoints.size()) {
    m_speedRules.assign(m_waypoints.size(), SpeedRule{ 1.0f, 150.f, 350.f });
  }
  resetPosition();
}


/**
 * @brief Establece reglas de velocidad para cada waypoint.
 * Si la cantidad de reglas no coincide con la cantidad de waypoints,
 * se ajusta automáticamente para igualar el tamaño.
 * @param rules Vector con las reglas de velocidad.
 */
void 
A_Racer::setSpeedRules(const std::vector<SpeedRule>& rules) {
  m_speedRules = rules;
  // Ajusta tamaño si difiere del número de waypoints
  if (m_speedRules.size() != m_waypoints.size() && !m_waypoints.empty()) {
    m_speedRules.resize(m_waypoints.size(), SpeedRule{ 1.0f, 150.f, 350.f });
  }
}

/**
 * @brief Reinicia la posición y estado del NPC.
 * Coloca al corredor en su waypoint inicial, reinicia vueltas y estado
 * de finalización, y aplica una velocidad inicial basada en la regla
 * del primer waypoint.
 */
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
    maybeApplySpeedRuleFor(currentWaypointIndex);
  }
}

/**
 * @brief Actualiza la lógica del NPC en cada frame.
 *
 * Si el corredor ha finalizado, solo actualiza sus componentes sin
 * movimiento adicional. De lo contrario, se mueve hacia el waypoint
 * actual.
 *
 * @param deltaTime Tiempo transcurrido desde el último frame (en segundos).
 */
void 
A_Racer::update(float deltaTime) {
  if (finished) { Actor::update(deltaTime); return; }

  Actor::update(deltaTime);
  if (!m_waypoints.empty()) {
    seekToCurrentWaypoint(deltaTime);
  }
}

/**
 * @brief Mueve al NPC hacia el waypoint actual.
 *
 * - Si llega al waypoint, avanza al siguiente.
 * - Si cruza la meta, incrementa el contador de vueltas.
 * - Puede cambiar su velocidad según la regla del waypoint.
 *
 * @param deltaTime Tiempo transcurrido desde el último frame (en segundos).
 */
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

/**
 * @brief Establece la velocidad actual del NPC.
 * @param newSpeed Nueva velocidad.
 */
void 
A_Racer::setSpeed(float newSpeed) { speed = newSpeed; }

/**
 * @brief Obtiene la distancia al siguiente waypoint.
 *
 * @return Distancia en píxeles al próximo waypoint.
 * Si no hay waypoints, devuelve un valor máximo.
 */
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

/**
 * @brief Aplica la regla de velocidad de un waypoint.
 *
 * - La velocidad cambia solo si la probabilidad `p` se cumple.
 * - El nuevo valor se elige aleatoriamente dentro del rango `[minS, maxS]`.
 *
 * @param waypointIndex Índice del waypoint para aplicar la regla.
 */
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
