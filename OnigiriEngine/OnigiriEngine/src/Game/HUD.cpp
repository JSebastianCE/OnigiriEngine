#include "HUD.h"
#include <imgui.h>

/**
 * @brief Muestra la lista de posiciones actuales en una ventana ImGui.
 *
 * Recorre la tabla `t` y dibuja cada posición con formato:
 * "posición° - nombre (Player) (Lap X)".
 *
 * @param t Vector de StandingRow con información de cada competidor.
 */
void 
HUD::drawStandings(const std::vector<StandingRow>& t) {
  ImGui::Begin("Posiciones");
  for (int i = 0; i < (int)t.size(); ++i) {
    ImGui::Text("%d° - %s%s  (Lap %d)",
      i + 1, t[i].name.c_str(), t[i].isPlayer ? " (Player)" : "",
      t[i].laps);
  }
  ImGui::End();
}

/**
 * @brief Dibuja un cronómetro en la interfaz.
 *
 * Convierte `s` a minutos, segundos y milisegundos. Se muestra en la parte
 * superior derecha con fondo translúcido y formato "MM:SS.mmm".
 *
 * @param s Tiempo total transcurrido en segundos.
 */
void 
HUD::drawTimer(float s) {
  int totalMs = (int)(s * 1000.0f);
  int minutes = totalMs / 60000; totalMs %= 60000;
  int seconds = totalMs / 1000;  totalMs %= 1000;
  int millis = totalMs;

  ImGui::SetNextWindowBgAlpha(0.35f);
  ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 320.f, 250.f), ImGuiCond_Always);

  ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
    ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoInputs;
  ImGui::Begin("CronometroHUD", nullptr, flags);
  ImGui::SetWindowFontScale(1.6f);
  ImGui::Text("%02d:%02d.%03d", minutes, seconds, millis);
  ImGui::End();
}

/**
 * @brief Muestra un overlay con el ganador y el podio de la carrera.
 *
 * Aparece centrado en pantalla y sin fondo cuando la carrera está congelada.
 *
 * @param frozen Si es `false`, no se dibuja nada.
 * @param winner Nombre del ganador de la carrera.
 * @param order Vector con el orden de llegada (máx. 3 primeros lugares).
 */
void 
HUD::drawWinnerOverlay(bool frozen, 
  const std::string& winner,
  const std::vector<std::string>& order) {
  if (!frozen) return;
  ImGui::SetNextWindowBgAlpha(0.0f);
  ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration |
    ImGuiWindowFlags_NoInputs |
    ImGuiWindowFlags_AlwaysAutoResize;

  ImGui::Begin("RaceOverlay", nullptr, flags);
  ImGui::SetWindowFontScale(2.5f); ImGui::Text("GANADOR...");
  ImGui::SetWindowFontScale(3.5f); ImGui::Text("%s", winner.c_str());
  ImGui::SetWindowFontScale(1.25f); ImGui::Separator();

  if (!order.empty()) {
    ImGui::Text("Podio:");
    if (order.size() >= 1) ImGui::Text("1) %s", order[0].c_str());
    if (order.size() >= 2) ImGui::Text("2) %s", order[1].c_str());
    if (order.size() >= 3) ImGui::Text("3) %s", order[2].c_str());
  }
  ImGui::End();
}

/**
 * @brief Muestra la velocidad actual del jugador en una ventana de depuración.
 *
 * Útil para comprobar en tiempo real si las reglas de velocidad funcionan correctamente.
 *
 * @param speed Velocidad actual del jugador en unidades de juego.
 */
void 
HUD::drawPlayerDebug(float speed) {
  ImGui::Begin("Debug Player");
  ImGui::Text("Player Speed: %.1f", speed);
  ImGui::End();
}
