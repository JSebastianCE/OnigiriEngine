#pragma once
#include <vector>
#include <string>
#include "RaceManager.h"


/**
 * @namespace HUD
 * @brief Funciones para dibujar elementos de interfaz relacionados con la carrera.
 *
 * Contiene utilidades para mostrar posiciones, cronómetro, podio de ganadores
 * y datos de depuración del jugador utilizando ImGui.
 */
namespace HUD {

  /**
   * @brief Dibuja la tabla de posiciones de la carrera.
   *
   * Muestra cada corredor con su posición, nombre, si es jugador y número de vueltas.
   *
   * @param table Vector de StandingRow con los datos actuales de la carrera.
   */
  void 
  drawStandings(const std::vector<StandingRow>& table);


  /**
   * @brief Dibuja un cronómetro con minutos, segundos y milisegundos.
   *
   * El cronómetro se posiciona en la esquina superior derecha con un fondo semitransparente.
   *
   * @param seconds Tiempo total transcurrido en segundos.
   */
  void 
  drawTimer(float seconds);

  /**
   * @brief Dibuja un overlay con el ganador y podio de la carrera.
   *
   * Sólo se muestra si la carrera está en estado congelado (`frozen = true`).
   *
   * @param frozen Indica si la carrera está detenida.
   * @param winner Nombre del corredor ganador.
   * @param finishOrder Vector con el orden de llegada (máx. 3 primeros para el podio).
   */
  void 
  drawWinnerOverlay(bool frozen, const std::string& winner,
  
  const std::vector<std::string>& finishOrder);

  /**
   * @brief Muestra la velocidad actual del jugador para depuración.
   *
   * Abre una ventana ImGui con el valor numérico de la velocidad.
   *
   * @param speed Velocidad actual del jugador.
   */
  void 
  drawPlayerDebug(float speed); 
}

