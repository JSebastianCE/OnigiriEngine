#pragma once

/**
 * @file BaseApp.h
 * @brief Declara la clase BaseApp, responsable del ciclo principal de la aplicación/juego:
 * inicialización, actualización por frame, renderizado y limpieza.
 */

#include <Window.h>
#include "CShape.h"
#include "ECS/Actor.h"
#include "EngineGUI.h"

#include <vector>
#include <string>


/// Subsistemas de gameplay (arquitectura modular)
#include "Game/WaypointSystem.h"
#include "Game/RaceManager.h"
#include "Game/A_Player.h"

/**
 * @class BaseApp
 * @brief Núcleo de la aplicación. Orquesta:
 *  - creación y administración de la ventana,
 *  - carga/configuración de actores (pista, jugador, NPCs),
 *  - inicialización y actualización de los sistemas de gameplay,
 *  - renderizado del mundo y HUD,
 *  - ciclo principal (run) y liberación de recursos.
 *
 * Esta clase intenta mantener el código de gameplay pesado
 * en módulos dedicados (RaceManager, WaypointSystem, HUD, Player/NPC).
 */

class
BaseApp {
public:
  /**
   * @brief Constructor por defecto. No realiza trabajo pesado.
   */
  BaseApp() = default;

  /**
   * @brief Destructor. Llama a @ref destroy indirectamente a través de @ref run.
   */
  ~BaseApp();

  /**
  * @brief Ejecuta el ciclo principal de la aplicación.
  * Secuencia: @ref init → bucle (eventos, @ref update, @ref render) → @ref destroy.
  * @return Código de salida del programa. 0 en éxito.
  */
  int  
  run();


  /**
   * @brief Inicializa recursos base y sistemas.
   * Crea ventana, inicializa GUI, crea pista y actores (player y NPCs),
   * define waypoints y configura los sistemas de gameplay.
   * @return true si la inicialización fue correcta; false si falló.
   */
  bool 
  init();


  /**
   * @brief Actualiza la lógica por frame.
   * - Actualiza deltaTime y GUI.
   * - Llama update() de todos los actores (player procesa WASD internamente).
   * - Llama @ref RaceManager::update para vueltas, ranking, cronómetro, etc.
   */
  void 
  update();


  /**
   * @brief Renderiza la escena por frame.
   * Dibuja la pista, los marcadores de waypoints, los actores y el HUD
   * (posiciones, cronómetro y overlay de ganador).
   */
  void 
  render();


  /**
   * @brief Libera recursos y apaga subsistemas.
   * Llama @ref EngineGUI::destroy. Los punteros inteligentes limpian el resto.
   */
  void 
  destroy();

private:
  // Mundo / render

   /**
   * @brief Ventana principal (SFML envuelta en Window).
   */
  EngineUtilities::TSharedPointer<Window> m_windowPtr;

  /**
   * @brief Shape auxiliar (reservado para pruebas o elementos sueltos).
   */
  EngineUtilities::TSharedPointer<CShape> m_shapePtr;

  /**
  * @brief Actor de la pista (Track). Renderiza el fondo/textura del circuito.
  */
  EngineUtilities::TSharedPointer<Actor>  m_Track;

  // Actores (player + NPCs)

  /**
   * @brief Lista general de actores renderizables/actualizables.
   * Incluye al jugador (como Actor) y a todos los NPCs.
   */
  std::vector<EngineUtilities::TSharedPointer<Actor>> m_actors;

  /**
   * @brief Referencia directa al jugador (A_Player).
   * Se añade también a @ref m_actors (como Actor) para tratarlos de forma uniforme.
   */
  EngineUtilities::TSharedPointer<A_Player> m_player; //  referencia directa al jugador

  // Waypoints (datos crudos + marcadores)


  /**
   * @brief Lista ordenada de posiciones 2D que definen el trazado del circuito.
   */
  std::vector<sf::Vector2f> m_waypoints;

  /**
 * @brief Marcadores visuales de los waypoints para debug.
 */
  std::vector<EngineUtilities::TSharedPointer<CShape>> m_waypointMarkers;

  /**
   * @brief Sistema de GUI (ImGui-SFML).
   */
  EngineGUI m_engineGUI;

  // Configuración de carrera

  /**
   * @brief Número total de vueltas de la carrera.
   */
  int m_totalLaps = 3;

  // Sistemas de gameplay 


  /**
   * @brief Sistema de waypoints (detección de paso, radio, marcadores).
   */
  WaypointSystem m_wps;

  /**
   * @brief Administrador de carrera (vueltas, ranking, cronómetro, freeze).
   */
  RaceManager m_race;
};
