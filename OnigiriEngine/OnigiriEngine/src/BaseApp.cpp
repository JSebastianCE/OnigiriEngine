#include "BaseApp.h"
#include "ResourceManager.h"
#include "EngineGUI.h"
#include <A_Racer.h>

#include <algorithm>   // std::sort
#include <cmath>       // std::sqrt

BaseApp::~BaseApp() {}

int BaseApp::run() {
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
  ResourceManager& resourceMan = ResourceManager::getInstance();

  m_windowPtr = EngineUtilities::MakeShared<Window>(1920, 1080, "Onigiri Engine");
  if (!m_windowPtr) {
    ERROR("BaseApp", "init", "Failed to create window pointer, check memory allocation");
    return false;
  }

  m_engineGUI.init(m_windowPtr);

  // Track 
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

  // Player (Yoshi) 
  m_ACircle = EngineUtilities::MakeShared<Actor>("Yoshi");
  if (!m_ACircle) {
    ERROR("BaseApp", "init", "Failed to create player Actor, check memory allocation");
    return false;
  }

  m_ACircle->getComponent<CShape>()->createShape(CIRCLE);
  m_ACircle->getComponent<CShape>()->setFillColor(sf::Color::White);
  m_ACircle->getComponent<Transform>()->setPosition(sf::Vector2f(415.f, 475.f));
  m_ACircle->getComponent<Transform>()->setScale(sf::Vector2f(3.f, 3));

  if (!resourceMan.loadTexture("Sprites/yoshi", "png")) {
    MESSAGE("BaseApp", "Init", "Can't load the texture");
  }
  m_ACircle->setTexture(resourceMan.getTexture("Sprites/yoshi"));
  m_actors.push_back(m_ACircle);

  //  Waypoints
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

  // Marcadores visuales
  for (const auto& wp : m_waypoints) {
    auto marker = EngineUtilities::MakeShared<CShape>();
    marker->createShape(ShapeType::CIRCLE);
    marker->setFillColor(sf::Color::Yellow);
    marker->setPosition(wp);
    marker->setScale(sf::Vector2f(1.f, 1.f));
    m_waypointMarkers.push_back(marker);
  }

  //  NPCs 
  auto npc1 = EngineUtilities::MakeShared<A_Racer>("Mario", 1);
  npc1->setWaypoints(m_waypoints);
  npc1->getComponent<CShape>()->createShape(CIRCLE);
  npc1->getComponent<CShape>()->setFillColor(sf::Color::White);
  npc1->getComponent<Transform>()->setScale(sf::Vector2f(3.f, 3.f));
  npc1->getComponent<Transform>()->setPosition(sf::Vector2f(415.f, 475.f));
  npc1->setSpeed(520.0f);
  if (!resourceMan.loadTexture("Sprites/Mario", "png")) {
    MESSAGE("BaseApp", "Init", "Can't load NPC texture");
  }
  npc1->setTexture(resourceMan.getTexture("Sprites/Mario"));
  m_actors.push_back(npc1);

  auto npc2 = EngineUtilities::MakeShared<A_Racer>("Luigi", 1);
  npc2->setWaypoints(m_waypoints);
  npc2->getComponent<CShape>()->createShape(CIRCLE);
  npc2->getComponent<CShape>()->setFillColor(sf::Color::White);
  npc2->getComponent<Transform>()->setScale(sf::Vector2f(3.f, 3.f));
  npc2->getComponent<Transform>()->setPosition(sf::Vector2f(415.f, 475.f));
  npc2->setSpeed(500.0f);
  if (!resourceMan.loadTexture("Sprites/Luigi", "png")) {
    MESSAGE("BaseApp", "Init", "Can't load NPC texture");
  }
  npc2->setTexture(resourceMan.getTexture("Sprites/Luigi"));
  m_actors.push_back(npc2);

  auto npc3 = EngineUtilities::MakeShared<A_Racer>("Peach", 1);
  npc3->setWaypoints(m_waypoints);
  npc3->getComponent<CShape>()->createShape(CIRCLE);
  npc3->getComponent<CShape>()->setFillColor(sf::Color::White);
  npc3->getComponent<Transform>()->setScale(sf::Vector2f(3.f, 3.f));
  npc3->getComponent<Transform>()->setPosition(sf::Vector2f(415.f, 475.f));
  npc3->setSpeed(510.0f);
  if (!resourceMan.loadTexture("Sprites/Peach", "png")) {
    MESSAGE("BaseApp", "Init", "Can't load NPC texture");
  }
  npc3->setTexture(resourceMan.getTexture("Sprites/Peach"));
  m_actors.push_back(npc3);

  auto npc4 = EngineUtilities::MakeShared<A_Racer>("DK", 1);
  npc4->setWaypoints(m_waypoints);
  npc4->getComponent<CShape>()->createShape(CIRCLE);
  npc4->getComponent<CShape>()->setFillColor(sf::Color::White);
  npc4->getComponent<Transform>()->setScale(sf::Vector2f(3.f, 3.f));
  npc4->getComponent<Transform>()->setPosition(sf::Vector2f(415.f, 475.f));
  npc4->setSpeed(515.0f);
  if (!resourceMan.loadTexture("Sprites/DK", "png")) {
    MESSAGE("BaseApp", "Init", "Can't load NPC texture");
  }
  npc4->setTexture(resourceMan.getTexture("Sprites/DK"));
  m_actors.push_back(npc4);


  // Estado de carrera
  m_totalLaps = 4;
  m_currentWaypointIndex = 0;
  m_prevPlayerWaypointIndex = m_currentWaypointIndex;
  m_playerLapCount = 0;
  m_playerFinished = false;
  m_raceFrozen = false;
  m_finishOrder.clear();
  m_winnerName.clear();

  //Cronometro
  m_raceClock.restart();
  m_elapsedTime = 0;
  m_raceFinished = false;

  return true;
}

void BaseApp::update() {
  if (!m_windowPtr.isNull()) m_windowPtr->update();

  // Cronómetro: si la carrera no ha terminado, refresca el tiempo
  if (!m_raceFinished) {
    m_elapsedTime = m_raceClock.getElapsedTime().asSeconds();
  }



  // GUI
  m_engineGUI.update(m_windowPtr, m_windowPtr->deltaTime);
  //ImGui::ShowDemoWindow(); 

  // Se permite movimiento? (se congela cuando llega el 3.º)
  const bool allowMovement = !m_raceFrozen;

  // Actualizar Track (solo sincroniza transform->shape)
  if (!m_Track.isNull()) {
    m_Track->update(m_windowPtr->deltaTime.asSeconds());
  }

  // Actualizar actores (NPCs: A_Racer maneja 'finished' internamente)
  for (auto& actor : m_actors) {
    if (!actor.isNull()) {
      // Si quieres frenar TODOS cuando hay freeze, no llames update a NPCs, pero
      // los dejamos actualizar para mantener visuales sincronizados:
      actor->update(m_windowPtr->deltaTime.asSeconds());
    }
  }

  //  Mover Yoshi + conteo de vueltas del jugador 
  float playerDistToNext = 0.0f;

  if (!m_waypoints.empty()) {
    if (allowMovement && !m_playerFinished) {
      // waypoint objetivo actual
      sf::Vector2f targetPos = m_waypoints[m_currentWaypointIndex];

      // posición actual
      sf::Vector2f currentPos = m_ACircle->getComponent<Transform>()->getPosition();

      // distancia al objetivo
      float dx = targetPos.x - currentPos.x;
      float dy = targetPos.y - currentPos.y;
      float distance = std::sqrt(dx * dx + dy * dy);

      // pasar al siguiente waypoint
      if (distance < 10.0f) {
        m_prevPlayerWaypointIndex = m_currentWaypointIndex;
        m_currentWaypointIndex++;
        if (m_currentWaypointIndex >= static_cast<int>(m_waypoints.size())) {
          m_currentWaypointIndex = 0;
        }
        // cruce de meta: del último -> 0
        if (m_prevPlayerWaypointIndex == static_cast<int>(m_waypoints.size()) - 1 &&
          m_currentWaypointIndex == 0) {
          m_playerLapCount++;
        }
        targetPos = m_waypoints[m_currentWaypointIndex];
      }

      // mover hacia el waypoint
      m_ACircle->getComponent<Transform>()->seek(
        m_waypoints[m_currentWaypointIndex],
        530.0f,
        m_windowPtr->deltaTime.asSeconds(),
        10.0f
      );

      if (!m_raceFinished) {
        m_elapsedTime = m_raceClock.getElapsedTime().asSeconds();
      }

    }

    // distancia del jugador al siguiente (para ranking)
    {
      sf::Vector2f posNow = m_ACircle->getComponent<Transform>()->getPosition();
      sf::Vector2f tgt = m_waypoints[m_currentWaypointIndex];
      float ddx = tgt.x - posNow.x;
      float ddy = tgt.y - posNow.y;
      playerDistToNext = std::sqrt(ddx * ddx + ddy * ddy);
    }
  }

  //  Marcar llegada de NPCs 
  if (allowMovement) {
    for (const auto& a : m_actors) {
      if (auto r = a.dynamic_pointer_cast<A_Racer>()) {
        if (!r->isFinished() && r->getLap() >= m_totalLaps) {
          r->markFinished(true);
          r->setSpeed(0.0f);
          m_finishOrder.push_back(r->getName());
        }
      }
    }

    //  Marcar llegada del jugador 
    if (!m_playerFinished && m_playerLapCount >= m_totalLaps) {
      m_playerFinished = true;
      m_finishOrder.push_back(m_ACircle->getName());
    }

    //  Congelar cuando haya 3 llegados 
    if (!m_raceFrozen && m_finishOrder.size() >= 3) {
      m_raceFrozen = true;
      m_winnerName = m_finishOrder.front();
      m_raceFinished = true;         // ← detiene el cronómetro
    }
  }

  //  RANKING (laps > waypoint > distancia). Incluye Yoshi. 
  struct Standing { std::string name; int laps; int wp; float dist; bool isPlayer; };
  std::vector<Standing> table;
  table.reserve(m_actors.size() + 1);

  // NPCs
  for (const auto& a : m_actors) {
    if (auto r = a.dynamic_pointer_cast<A_Racer>()) {
      table.push_back({ r->getName(), r->getLap(), r->getCurrentWaypointIndex(), r->getDistanceToNextWaypoint(), false });
    }
  }
  // Player
  table.push_back({ m_ACircle->getName(), m_playerLapCount, m_currentWaypointIndex, playerDistToNext, true });

  std::sort(table.begin(), table.end(),
    [](const Standing& A, const Standing& B) {
      if (A.laps != B.laps) return A.laps > B.laps;
      if (A.wp != B.wp) return A.wp > B.wp;
      return A.dist < B.dist;
    });

  //  HUD posiciones 
  ImGui::Begin("Posiciones");
  for (int i = 0; i < static_cast<int>(table.size()); ++i) {
    ImGui::Text("%d° - %s%s  (Lap %d)", i + 1, table[i].name.c_str(), table[i].isPlayer ? " (Player)" : "", table[i].laps);
  }
  ImGui::End();

  // Overlay de ganador y podio 
  if (m_raceFrozen) {
    // Ventana transparente, sin bordes, sin inputs
    ImGui::SetNextWindowBgAlpha(0.0f);
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs |
      ImGuiWindowFlags_AlwaysAutoResize;
    ImGui::Begin("RaceOverlay", nullptr, flags);

    ImGui::SetWindowFontScale(2.5f);
    ImGui::Text("GANADOR...");
    ImGui::SetWindowFontScale(3.5f);
    ImGui::Text("%s", m_winnerName.c_str());
    ImGui::SetWindowFontScale(1.25f);
    ImGui::Separator();
    if (!m_finishOrder.empty()) {
      ImGui::Text("Podio:");
      if (m_finishOrder.size() >= 1) ImGui::Text("1) %s", m_finishOrder[0].c_str());
      if (m_finishOrder.size() >= 2) ImGui::Text("2) %s", m_finishOrder[1].c_str());
      if (m_finishOrder.size() >= 3) ImGui::Text("3) %s", m_finishOrder[2].c_str());
    }

    ImGui::End();
  }
    // ===== HUD: Cronómetro =====
  {
    int totalMs = static_cast<int>(m_elapsedTime * 1000.0f);
    int minutes =  totalMs / 60000;  totalMs %= 60000;
    int seconds =  totalMs / 1000;   totalMs %= 1000;
    int millis  =  totalMs;

    // (Opcional) esquina superior derecha
    ImGui::SetNextWindowBgAlpha(0.35f);
    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 320.0f, 250.0f), ImGuiCond_Always);

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
                             ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoInputs;
    ImGui::Begin("CronometroHUD", nullptr, flags);
    ImGui::SetWindowFontScale(1.6f);
    ImGui::Text("%02d:%02d.%03d", minutes, seconds, millis);
    ImGui::End();
  }
}

void BaseApp::render() {
  if (!m_windowPtr) return;

  m_windowPtr->clear();

  if (!m_Track.isNull()) {
    m_Track->getComponent<CShape>()->render(m_windowPtr);
  }

  for (const auto& marker : m_waypointMarkers) {
    marker->render(m_windowPtr);
  }

  if (m_shapePtr) m_shapePtr->render(m_windowPtr);

  for (auto& actor : m_actors) {
    if (!actor.isNull()) {
      actor->render(m_windowPtr);
    }
  }

  m_windowPtr->render();
  m_engineGUI.render(m_windowPtr);
  m_windowPtr->display();
}

void BaseApp::destroy() {
  m_engineGUI.destroy();
}
