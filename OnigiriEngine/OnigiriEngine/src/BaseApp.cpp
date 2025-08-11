/**
 * @file BaseApp.cpp
 * @brief Implementa la clase BaseApp. Se encarga de:
 *  - crear ventana y GUI,
 *  - crear pista, waypoints y actores (player/NPCs),
 *  - inicializar y actualizar sistemas de gameplay (WaypointSystem, RaceManager),
 *  - ejecutar el ciclo principal y dibujar el HUD.
 */

#include "BaseApp.h"
#include "ResourceManager.h"
#include "EngineGUI.h"
#include <A_Racer.h>

#include <algorithm>
#include <cmath>
#include <random>

#include "Game/HUD.h"

BaseApp::~BaseApp() {}

int BaseApp::run() {
  /**
  * @brief Punto de entrada del ciclo principal.
  * - Llama a @ref init. Si falla, registra error.
  * - Mientras la ventana esté abierta:
  *    - procesa eventos (entrada, GUI),
  *    - actualiza lógica (@ref update),
  *    - renderiza la escena (@ref render).
  * - Una vez termina, llama a @ref destroy.
  */
  if (!init()) {
    ERROR("BaseApp", "run", "Initializes result on a false statement", "check method validations");
  }
  while (m_windowPtr->isOpen()) {
    m_windowPtr->handleEvents(m_engineGUI);
    update();
    render();
  }
  destroy();
  return 0;
}

bool 
BaseApp::init() {
  /**
  * @brief Inicializa recursos, crea actores y configura sistemas.
  *
  * Flujo principal:
  * 1) Crear ventana y GUI.
  * 2) Crear pista y asignar textura.
  * 3) Definir lista de waypoints del circuito.
  * 4) Configurar WaypointSystem con radio de detección y crear marcadores.
  * 5) Crear Player (A_Player): componentes, textura, speed/base y waypointRadius.
  * 6) Crear NPCs (A_Racer) y opcionalmente sus reglas de velocidad por waypoint.
  * 7) Inicializar RaceManager con total de vueltas, player, actores y WaypointSystem.
  */
  ResourceManager& resourceMan = ResourceManager::getInstance();

  // Ventana
  m_windowPtr = EngineUtilities::MakeShared<Window>(1920, 1080, "Onigiri Engine");
  if (!m_windowPtr) {
    ERROR("BaseApp", "init", "Failed to create window pointer, check memory allocation");
    return false;
  }
  m_engineGUI.init(m_windowPtr);

  // Track (pista) como Actor
  m_Track = EngineUtilities::MakeShared<Actor>("Track Actor");
  if (m_Track) {
    m_Track->getComponent<CShape>()->createShape(RECTANGLE);
    m_Track->getComponent<CShape>()->setFillColor(sf::Color::White);
    m_Track->getComponent<Transform>()->setPosition(sf::Vector2f(350.f, 0.f));
    m_Track->getComponent<Transform>()->setScale(sf::Vector2f(17.0f, 22.0f));
    if (!resourceMan.loadTexture("Sprites/Track", "png")) {
      MESSAGE("BaseApp", "Init", "Can't load the texture");
    }
    m_Track->setTexture(resourceMan.getTexture("Sprites/Track"));
  }

  //  Waypoints del circuito  
  m_waypoints.clear();
  m_waypoints.push_back(sf::Vector2f(415.f, 200.f));
  m_waypoints.push_back(sf::Vector2f(445.f, 120.f));
  m_waypoints.push_back(sf::Vector2f(500.f, 100.f));
  m_waypoints.push_back(sf::Vector2f(1200.f, 100.f));
  m_waypoints.push_back(sf::Vector2f(1300.f, 110.f));
  m_waypoints.push_back(sf::Vector2f(1325.f, 175.f));
  m_waypoints.push_back(sf::Vector2f(1325.f, 400.f));
  m_waypoints.push_back(sf::Vector2f(1275.f, 475.f));
  m_waypoints.push_back(sf::Vector2f(795.f, 475.f));
  m_waypoints.push_back(sf::Vector2f(775.f, 675.f));
  m_waypoints.push_back(sf::Vector2f(825.f, 700.f));
  m_waypoints.push_back(sf::Vector2f(1525.f, 700.f));
  m_waypoints.push_back(sf::Vector2f(1600.f, 725.f));
  m_waypoints.push_back(sf::Vector2f(1600.f, 925.f));
  m_waypoints.push_back(sf::Vector2f(1525.f, 975.f));
  m_waypoints.push_back(sf::Vector2f(1000.f, 975.f));
  m_waypoints.push_back(sf::Vector2f(600.f, 975.f));
  m_waypoints.push_back(sf::Vector2f(500.f, 975.f));
  m_waypoints.push_back(sf::Vector2f(450.f, 960.f));
  m_waypoints.push_back(sf::Vector2f(415.f, 495.f));

  // WaypointSystem: radio de detección y marcadores
  // - Deteccion del paso.
  // - buildMarkers crea círculos amarillos para depurar.
  m_wps.setWaypoints(m_waypoints, 48.f);
  m_wps.buildMarkers(m_waypointMarkers);

  // Player (A_Player)
  // - El control WASD se procesa dentro de A_Player::update().
  // - setWaypointRadius alinea la detección interna del player con el sistema global.
  m_player = EngineUtilities::MakeShared<A_Player>("Yoshi");
  if (!m_player) {
    ERROR("BaseApp", "init", "Failed to create player", "");
    return false;
  }
  m_player->getComponent<CShape>()->createShape(CIRCLE);
  m_player->getComponent<CShape>()->setFillColor(sf::Color::White);
  m_player->getComponent<Transform>()->setScale(sf::Vector2f(3.f, 3.f));
  m_player->getComponent<Transform>()->setPosition(sf::Vector2f(415.f, 475.f));
  if (!resourceMan.loadTexture("Sprites/yoshi", "png")) {
    MESSAGE("BaseApp", "Init", "Can't load the texture");
  }
  m_player->setTexture(resourceMan.getTexture("Sprites/yoshi"));
  // asignar waypoints al player (internamente lleva su conteo y entrada WASD)
  m_player->setWaypoints(m_waypoints);
  m_player->setBaseSpeed(300.f); // escala de velocidad al presionar WASD
  m_player->setWaypointRadius(48.f);

  // Añadir player como Actor a la lista general
  m_actors.push_back(m_player.dynamic_pointer_cast<Actor>());

  //  NPCs (A_Racer) – pequeña factoría local con textura y velocidad base
  auto makeNPC = [&](const char* name, const char* tex, float speed) {
    auto npc = EngineUtilities::MakeShared<A_Racer>(name, 1);
    npc->setWaypoints(m_waypoints);
    npc->getComponent<CShape>()->createShape(CIRCLE);
    npc->getComponent<CShape>()->setFillColor(sf::Color::White);
    npc->getComponent<Transform>()->setScale(sf::Vector2f(3.f, 3.f));
    npc->getComponent<Transform>()->setPosition(sf::Vector2f(415.f, 475.f));
    npc->setSpeed(speed);
    if (!resourceMan.loadTexture(std::string("Sprites/") + tex, "png")) {
      MESSAGE("BaseApp", "Init", "Can't load NPC texture");
    }
    npc->setTexture(resourceMan.getTexture(std::string("Sprites/") + tex));
    m_actors.push_back(npc);
    return npc;
    };

  auto npc1 = makeNPC("Mario", "Mario", 320.f);
  auto npc2 = makeNPC("Luigi", "Luigi", 300.f);
  auto npc3 = makeNPC("Peach", "Peach", 310.f);
  auto npc4 = makeNPC("DK", "DK", 315.f);

  // Reglas de velocidad por waypoint para NPCs (Modificable)
  {
    std::vector<A_Racer::SpeedRule> rules(m_waypoints.size(), { 1.0f, 150.f, 350.f });

    if (rules.size() > 10) rules[10] = { 0.4f, 160.f, 260.f };
    for (auto& a : m_actors) {
      if (auto r = a.dynamic_pointer_cast<A_Racer>()) r->setSpeedRules(rules);
    }
  }

  // RaceManager: inicializa la carrera (vueltas, referencias y cronómetro)
  m_totalLaps = 4;
  m_race.init(m_totalLaps, m_player, m_actors, &m_wps);
  m_race.start();

  return true;
}

void 
BaseApp::update() {
  /**
  * @brief Actualización por frame:
  *  - Actualiza deltaTime y GUI (ImGui-SFML).
  *  - Sincroniza el Track (Transform → Shape).
  *  - Llama update() de todos los actores (el Player procesa WASD).
  *  - Delegación al RaceManager para control de vueltas, llegadas, ranking y freeze.
  */
  if (!m_windowPtr.isNull()) m_windowPtr->update();

  // GUI (ImGui-SFML)
  m_engineGUI.update(m_windowPtr, m_windowPtr->deltaTime);

  // Sincroniza track (Transform->Shape)
  if (!m_Track.isNull()) {
    m_Track->update(m_windowPtr->deltaTime.asSeconds());
  }

  // Actualizar todos los actores (player y NPCs)
  for (auto& actor : m_actors) {
    if (!actor.isNull()) actor->update(m_windowPtr->deltaTime.asSeconds());
  }

  // Lógica de carrera (vueltas, llegadas, ranking, cronómetro)
  m_race.update(m_windowPtr->deltaTime.asSeconds());

  // Regla de finalización: congela al llegar los 3 primeros
  m_race.freezeIfTopNArrived(3); // congela al llegar los 3 primeros
}

void 
BaseApp::render() {
  /**
 * @brief Render por frame:
 *  - Limpia la pantalla.
 *  - Dibuja la pista, los marcadores de waypoint, los actores.
 *  - Dibuja HUD: posiciones, cronómetro y overlay de ganador.
 *  - Presenta el frame.
 */
  if (!m_windowPtr) return;

  m_windowPtr->clear();

  if (!m_Track.isNull()) {
    m_Track->getComponent<CShape>()->render(m_windowPtr);
  }

  // Waypoint markers (debug)
  for (const auto& marker : m_waypointMarkers) {
    marker->render(m_windowPtr);
  }

  if (m_shapePtr) m_shapePtr->render(m_windowPtr);

  // Dibujar actores
  for (auto& actor : m_actors) {
    if (!actor.isNull()) actor->render(m_windowPtr);
  }

  // HUD (posiciones, cronómetro, ganador)
  HUD::drawStandings(m_race.standings());
  HUD::drawTimer(m_race.elapsedSeconds());
  HUD::drawWinnerOverlay(m_race.frozen(), m_race.winner(), m_race.finishOrder());
  // HUD::drawPlayerDebug(m_player ? m_player->getSpeed() : 0.f); // si necesitas debug

  m_windowPtr->render();
  m_engineGUI.render(m_windowPtr);
  m_windowPtr->display();
}

void 
BaseApp::destroy() {
  /**
   * @brief Apaga la GUI y deja que los punteros inteligentes liberen recursos.
   */
  m_engineGUI.destroy();
}
