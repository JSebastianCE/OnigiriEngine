#include "Window.h"

/**
 * @class Window
 * @brief Encapsulates an SFML window, handling creation, events, rendering, and destruction.
 */

 /**
  * @brief Constructs a new Window object.
  *
  * Initializes an SFML render window with the specified width, height, and title.
  * It also sets the framerate limit and verifies successful creation.
  *
  * @param width Width of the window in pixels.
  * @param height Height of the window in pixels.
  * @param title Title of the window.
  */
Window::Window(int width, int height, const std::string& title) {
  m_window = new sf::RenderWindow(sf::VideoMode(width, height), title);

  if (m_window) {
    m_window->setFramerateLimit(60);
    MESSAGE("Window", "Window", "Window created succesfully");
  }
  else {
    ERROR("Window", "Window", "Failed to create window");
  }
}

/**
 * @brief Destroys the Window object and safely releases its resources.
 */
Window::~Window() {
  SAFE_PTR_RELEASE(m_window);
}

/**
 * @brief Handles window events such as closing.
 *
 * Processes the event queue to detect and handle user actions like closing the window.
 */
void Window::handleEvents() {
  sf::Event event;
  while (m_window->pollEvent(event)) {
    if (event.type == sf::Event::Closed) {
      m_window->close();
    }
  }
}

/**
 * @brief Checks if the window is currently open.
 *
 * @return true if the window is open, false otherwise.
 */
bool Window::isOpen() const {
  if (m_window) {
    return m_window->isOpen();
  }
  else {
    ERROR("Window", "isOpen", "Window is null");
    return false;
  }
}

/**
 * @brief Clears the window with a specific background color.
 *
 * @param color The color to use when clearing the window.
 */
void Window::clear(const sf::Color& color) {
  if (m_window) {
    m_window->clear(color);
  }
  else {
    ERROR("Window", "clear", "Window is null");
  }
}

/**
 * @brief Draws a drawable object to the window using specified render states.
 *
 * @param drawable The SFML drawable object to render.
 * @param states Optional render states to apply to the drawable.
 */
void Window::draw(const sf::Drawable& drawable, const sf::RenderStates& states){
  if (m_window) {
    m_window->draw(drawable, states);
  }
  else {
    ERROR("Window", "draw", "Window is null");
  }
}

/**
 * @brief Displays the contents of the current frame on the screen.
 */
void Window::display() {
  if (m_window) {
    m_window->display();
  }
  else {
    ERROR("Window", "display", "Window is null");
  }
}

/**
 * @brief Destroys the window and releases its resources safely.
 */
void Window::destroy() {
  SAFE_PTR_RELEASE(m_window);
}
