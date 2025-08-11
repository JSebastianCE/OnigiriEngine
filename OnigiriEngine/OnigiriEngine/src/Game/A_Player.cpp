/**
 * @file A_Player.cpp
 * @brief Implementación del actor controlado por el jugador (WASD), con detección de waypoints,
 *        conteo de vueltas y reglas de velocidad por probabilidad.
 */

#include "A_Player.h"
#include "ECS/Transform.h"

#include <SFML/Window/Keyboard.hpp>   // ← teclas (SFML 3)
#include <algorithm>                   // std::min/max
#include <cmath>
//#include "EngineUtilities/EngineMath.h"
#include <random>

/**
 * @brief Generador de números aleatorios local al archivo para las reglas de velocidad.
 *
 * Se inicializa una sola vez (patrón singleton estático local) y se reutiliza para
 * evitar el costo de re-seed en cada llamada.
 *
 * @return Referencia a un std::mt19937 listo para usarse.
 */
static 
std::mt19937& rng_player() {
  static 
  std::mt19937 eng{ std::random_device{}() };
  return eng;
}


/**
 * @brief Constructor: delega en Actor para fijar el nombre.
 * @param name Nombre identificador del jugador.
 */
A_Player::A_Player(const std::string& name)
  : Actor(name) {
}

/**
 * @brief Asigna el conjunto de waypoints que definen el circuito para el jugador.
 *
 * También:
 *  - Ajusta el vector de reglas de velocidad si su tamaño no coincide con el de los waypoints.
 *  - Reinicia el estado de carrera (vueltas, índices).
 *  - Coloca al jugador en el primer waypoint si existe Transform y la lista no está vacía.
 *
 * @param wps Vector de posiciones 2D (sf::Vector2f) que representan los waypoints en orden.
 */
void 
A_Player::setWaypoints(const std::vector<sf::Vector2f>& wps) {
  m_waypoints = wps;
  if (m_speedRules.size() != m_waypoints.size()) {
    m_speedRules.assign(m_waypoints.size(), SpeedRule{ 1.0f, 200.f, 350.f });
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

/**
 * @brief Establece reglas de velocidad para cada waypoint.
 *
 * Si el número de reglas no coincide con la cantidad de waypoints (y estos existen),
 * se redimensiona llenando con valores por defecto.
 *
 * @param rules Vector con las reglas {probabilidad, minVel, maxVel} por waypoint.
 */
void 
A_Player::setSpeedRules(const std::vector<SpeedRule>& rules) {
  m_speedRules = rules;
  if (!m_waypoints.empty() && m_speedRules.size() != m_waypoints.size()) {
    m_speedRules.resize(m_waypoints.size(), SpeedRule{ 1.0f, 250.f, 350.f });
  }
}

/**
 * @brief Define la velocidad base del jugador y sincroniza la velocidad actual.
 * @param s Velocidad base inicial.
 */
void 
A_Player::setBaseSpeed(float s) {
  m_baseSpeed = s;
  m_speed = s;
}


/**
 * @brief Lee el estado del teclado (WASD) usando Scancodes de SFML 3 y devuelve un vector dirección normalizado.
 *
 * Se emplean scancodes (layout-independientes), por lo que funciona igual con distintos
 * teclados/idiomas. Si no hay teclas presionadas, retorna (0,0).
 *
 * @return sf::Vector2f dirección normalizada; longitud 1 si hay input, 0 si no.
 */
sf::Vector2f A_Player::getDirFromKeys() const {

  using 
  KC = sf::Keyboard::Scancode;     //  alias para scancodes (layout-independiente)

  float x = 0.f, y = 0.f;
  if (sf::Keyboard::isKeyPressed(KC::W)) y -= 1.f;
  if (sf::Keyboard::isKeyPressed(KC::S)) y += 1.f;
  if (sf::Keyboard::isKeyPressed(KC::A)) x -= 1.f;
  if (sf::Keyboard::isKeyPressed(KC::D)) x += 1.f;

  sf::Vector2f v{ x, y };

  float 
  len = std::sqrt(v.x * v.x + v.y * v.y);
  if (len > 0.f) {
    v.x /= len;
    v.y /= len;
  }
  return v; // (0,0) si no hay teclas
}

/**
 * @brief Actualiza el estado del jugador por frame.
 *
 * - Sincroniza Transform → Shape (vía Actor::update).
 * - Si no terminó y hay waypoints, procesa entrada (WASD) y movimiento.
 * - Comprueba cruce del waypoint actual usando un radio de detección (m_wpRadius).
 *
 * @param deltaTime Tiempo transcurrido en segundos desde el último frame.
 */
void 
A_Player::update(float deltaTime) {
  Actor::update(deltaTime); // sincroniza Transform->Shape

  if (m_finished || m_waypoints.empty()) return;

  handleInput(deltaTime);

  // Checar cruce de waypoint
  auto t = getComponent<Transform>();
  if (!t) return;

  sf::Vector2f pos = t->getPosition();
  sf::Vector2f target = m_waypoints[m_currentWp];

  float 
  dx = target.x - pos.x;

  float 
  dy = target.y - pos.y;

  float
  dist = std::sqrt(dx * dx + dy * dy);

  if (dist < m_wpRadius) {
    onReachNextWaypoint();
  }
}

/**
 * @brief Aplica el movimiento en función del input del usuario.
 *
 * Si no hay input (dirección 0,0), no se desplaza. En caso contrario, traslada
 * la posición actual en la dirección normalizada multiplicada por m_speed y dt.
 *
 * @param dt Delta de tiempo del frame.
 */
void 
A_Player::handleInput(float dt) {
  auto t = getComponent<Transform>();
  if (!t) return;

  sf::Vector2f dir = getDirFromKeys();
  if (dir.x == 0.f && dir.y == 0.f) {
    // Si no presionas nada, no avanzas.
    return;
  }

  // Movimiento simple (sin aceleración)
  sf::Vector2f pos = t->getPosition();

  pos += dir * m_speed * dt;
  t->setPosition(pos);
}

/**
 * @brief Gestiona el evento de alcanzar un waypoint.
 *
 * - Avanza al siguiente waypoint de forma cíclica.
 * - Si pasó del último al primero, incrementa la vuelta (lap).
 * - Llama a la aplicación de regla de velocidad (probabilística) para el nuevo waypoint.
 */
void 
A_Player::onReachNextWaypoint() {
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

/**
 * @brief Aplica (si corresponde) el cambio de velocidad definido por la regla del waypoint dado.
 *
 * Toma la regla del índice indicado:
 *  - Lanza una moneda con probabilidad @c p .
 *  - Si sucede, elige una nueva velocidad uniforme en [minS, maxS].
 *
 * @param wpIndex Índice del waypoint para el cual evaluar la regla.
 */
void 
A_Player::maybeApplySpeedRuleFor(int wpIndex) {
  if (m_speedRules.empty()) return;
  const auto& r = m_speedRules[wpIndex];

  std::uniform_real_distribution<float> coin(0.f, 1.f);

  if (coin(rng_player()) <= r.p) {
    float 
    lo = std::min(r.minS, r.maxS);

    float 
    hi = std::max(r.minS, r.maxS);

    std::uniform_real_distribution<float> pick(lo, hi);

    m_speed = pick(rng_player());
  }
}

/**
 * @brief Calcula la distancia actual al siguiente waypoint.
 *
 * Útil para desempatar posiciones cuando dos corredores están en el mismo índice
 * de waypoint: menor distancia implica estar “más adelante”.
 *
 * @return Distancia en píxeles; si no hay Transform o waypoints, retorna un valor grande (1e9).
 */
float 
A_Player::getDistanceToNextWaypoint() const {
  auto t = const_cast<A_Player*>(this)->getComponent<Transform>();
  if (!t || m_waypoints.empty()) return 1e9f;

  sf::Vector2f pos = t->getPosition();
  sf::Vector2f target = m_waypoints[m_currentWp];

  float 
  dx = target.x - pos.x;

  float 
  dy = target.y - pos.y;

  return std::sqrt(dx * dx + dy * dy);
}
