#include "EngineGUI.h"
#include "Window.h"

void
EngineGUI::init(const EngineUtilities::TSharedPointer<Window>& window) {
	// Initalize the ImGui Resource
	ImGui::SFML::Init(*window->m_windowPtr);
	setupDarkGUIStyle();
}

void
EngineGUI::update(const EngineUtilities::TSharedPointer<Window>& window,
	sf::Time deltaTime) {
	ImGui::SFML::Update(*window->m_windowPtr, deltaTime);
	barMenu();
}

void
EngineGUI::render(const EngineUtilities::TSharedPointer<Window>& window) {
	ImGui::SFML::Render(*window->m_windowPtr);
}
void
EngineGUI::processEvent(const sf::Window& window, const sf::Event& event) {
	ImGui::SFML::ProcessEvent(window, event);
}

void 
EngineGUI::setupDarkGUIStyle() {
  ImGuiStyle& style = ImGui::GetStyle();

  // General spacing y bordes
  style.WindowPadding = ImVec2(10, 10);
  style.FramePadding = ImVec2(6, 12);
  style.ItemSpacing = ImVec2(12, 8);
  style.WindowRounding = 6.0f;
  style.FrameRounding = 4.0f;
  style.ScrollbarRounding = 4.0f;
  style.GrabRounding = 4.0f;
  style.WindowBorderSize = 1.0f;
  style.FrameBorderSize = 1.0f;

  ImVec4* colors = style.Colors;

  // Colores base
  ImVec4 transparentDark = ImVec4(0.04f, 0.04f, 0.04f, 0.85f);  // Fondo de ventanas
  ImVec4 lightGray = ImVec4(0.7f, 0.7f, 0.7f, 0.25f);           // Cuadros y botones
  ImVec4 lightGrayHover = ImVec4(0.8f, 0.8f, 0.8f, 0.35f);
  ImVec4 lightGrayActive = ImVec4(0.9f, 0.9f, 0.9f, 0.45f);
  ImVec4 whiteBorder = ImVec4(1.0f, 1.0f, 1.0f, 0.7f);
  ImVec4 text = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

  // Texto
  colors[ImGuiCol_Text] = text;
  colors[ImGuiCol_TextDisabled] = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);

  // Fondos
  colors[ImGuiCol_WindowBg] = transparentDark;
  colors[ImGuiCol_ChildBg] = ImVec4(0.03f, 0.03f, 0.03f, 0.80f);
  colors[ImGuiCol_PopupBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.9f);

  // Bordes
  colors[ImGuiCol_Border] = whiteBorder;
  colors[ImGuiCol_BorderShadow] = ImVec4(0, 0, 0, 0.0f);

  // Cuadros
  colors[ImGuiCol_FrameBg] = lightGray;
  colors[ImGuiCol_FrameBgHovered] = lightGrayHover;
  colors[ImGuiCol_FrameBgActive] = lightGrayActive;

  // Título
  colors[ImGuiCol_TitleBg] = transparentDark;
  colors[ImGuiCol_TitleBgActive] = transparentDark;
  colors[ImGuiCol_TitleBgCollapsed] = transparentDark;

  // Menú
  colors[ImGuiCol_MenuBarBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.9f);

  // Scrollbars
  colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.7f);
  colors[ImGuiCol_ScrollbarGrab] = lightGray;
  colors[ImGuiCol_ScrollbarGrabHovered] = lightGrayHover;
  colors[ImGuiCol_ScrollbarGrabActive] = lightGrayActive;

  // Sliders y botones
  colors[ImGuiCol_SliderGrab] = whiteBorder;
  colors[ImGuiCol_SliderGrabActive] = ImVec4(1.0f, 1.0f, 1.0f, 0.9f);
  colors[ImGuiCol_Button] = lightGray;
  colors[ImGuiCol_ButtonHovered] = lightGrayHover;
  colors[ImGuiCol_ButtonActive] = lightGrayActive;

  // Tabs
  colors[ImGuiCol_Tab] = lightGray;
  colors[ImGuiCol_TabHovered] = lightGrayHover;
  colors[ImGuiCol_TabActive] = lightGrayActive;
  colors[ImGuiCol_TabUnfocused] = lightGray;
  colors[ImGuiCol_TabUnfocusedActive] = lightGrayActive;

  // Headers (para collapsing headers, tablas, etc.)
  colors[ImGuiCol_Header] = lightGray;
  colors[ImGuiCol_HeaderHovered] = lightGrayHover;
  colors[ImGuiCol_HeaderActive] = lightGrayActive;

  // Otros
  colors[ImGuiCol_Separator] = whiteBorder;
  colors[ImGuiCol_ResizeGrip] = whiteBorder;
  colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.0f, 1.0f, 1.0f, 0.9f);
  colors[ImGuiCol_ResizeGripActive] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
  colors[ImGuiCol_CheckMark] = whiteBorder;
  colors[ImGuiCol_TextSelectedBg] = ImVec4(1.0f, 1.0f, 1.0f, 0.15f);
  colors[ImGuiCol_DragDropTarget] = ImVec4(1.0f, 1.0f, 1.0f, 0.3f);

  colors[ImGuiCol_NavHighlight] = ImVec4(1.0f, 0.55f, 0.0f, 0.35f); // naranja transparente

  colors[ImGuiCol_HeaderHovered] = ImVec4(1.0f, 0.55f, 0.0f, 0.25f);  // Para ítems tipo lista
  colors[ImGuiCol_FrameBgHovered] = ImVec4(1.0f, 0.55f, 0.0f, 0.25f); // Para cuadros (inputs, etc.)
  colors[ImGuiCol_ButtonHovered] = ImVec4(1.0f, 0.55f, 0.0f, 0.25f);  // Para botones

}



void 
EngineGUI::barMenu() {
  if (ImGui::BeginMainMenuBar()) {
    // Estilo personalizado para el menú
    ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.7f, 0.7f, 0.7f, 0.2f));        // Fondo normal de ítem
    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(1.0f, 0.55f, 0.0f, 0.35f)); // Hover naranja translúcido
    ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(1.0f, 0.55f, 0.0f, 0.5f));   // Activo más opaco

    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f)); // Texto blanco

    // Menú Archivo
    if (ImGui::BeginMenu("File")) {
      ImGui::MenuItem("New");
      ImGui::MenuItem("Open", "Ctrl+O");
      ImGui::MenuItem("Save", "Ctrl+S");
      ImGui::MenuItem("Save As...");
      ImGui::Separator();
      if (ImGui::MenuItem("Exit", "Alt+F4")) exit(0);
      ImGui::EndMenu();
    }

    // Menú Editar
    if (ImGui::BeginMenu("Edit")) {
      ImGui::MenuItem("Undo", "Ctrl+Z");
      ImGui::MenuItem("Redo", "Ctrl+Y");
      ImGui::Separator();
      ImGui::MenuItem("Cut", "Ctrl+X");
      ImGui::MenuItem("Copy", "Ctrl+C");
      ImGui::MenuItem("Paste", "Ctrl+V");
      ImGui::EndMenu();
    }

    // Menú Ver
    if (ImGui::BeginMenu("View")) {
      ImGui::MenuItem("Show Console");
      ImGui::MenuItem("Show Hierarchy");
      ImGui::EndMenu();
    }

    // Menú Herramientas
    if (ImGui::BeginMenu("Tools")) {
      ImGui::MenuItem("Options");
      ImGui::EndMenu();
    }

    // Menú Ayuda
    if (ImGui::BeginMenu("Help")) {
      ImGui::MenuItem("Documentation");
      ImGui::MenuItem("About");
      ImGui::EndMenu();
    }

    // Restaurar estilo
    ImGui::PopStyleColor(4);
    ImGui::EndMainMenuBar();
  }
}


void
EngineGUI::destroy() {
	ImGui::SFML::Shutdown();
}