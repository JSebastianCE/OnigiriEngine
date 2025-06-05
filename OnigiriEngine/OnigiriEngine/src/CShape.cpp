#include "CShape.h"
#include "Window.h"

/**
 * @file CShape.cpp
 * @brief Implementation of the CShape class for creating and manipulating different SFML shapes.  
 */

 /**
  * @brief Creates a shape of the specified type.
  *
  * Allocates and configures a shape (Circle, Rectangle, Triangle, or Polygon) based on the given shape type.
  * The shape is stored internally and returned as a pointer to sf::Shape.
  *
  * @param shapeType The type of shape to create.
  * @return sf::Shape* Pointer to the created shape, or nullptr if the type is invalid.
  */
sf::Shape* CShape::createShape(ShapeType shapeType) {
  m_shapeType = shapeType;

  switch (shapeType) {
  case ShapeType::CIRCLE: {
    sf::CircleShape* circle = new sf::CircleShape(10.f);
    circle->setFillColor(sf::Color::White);
    m_shape = circle;
    return circle;
  }
  case ShapeType::RECTANGLE: {
    sf::RectangleShape* rectangle = new sf::RectangleShape(sf::Vector2f(100.f,
      50.f));
    rectangle->setFillColor(sf::Color::White);
    m_shape = rectangle;
    return rectangle;
  }
  case ShapeType::TRIANGLE: {
    sf::ConvexShape* triangle = new sf::ConvexShape(3);
    triangle->setPoint(0, sf::Vector2f(0.f, 0.f));
    triangle->setPoint(1, sf::Vector2f(50.f, 100.f));
    triangle->setPoint(2, sf::Vector2f(100.f, 0.f));
    triangle->setFillColor(sf::Color::White);
    m_shape = triangle;
    return triangle;
  }
  case ShapeType::POLYGON: {
    sf::ConvexShape* polygon = new sf::ConvexShape(5);
    polygon->setPoint(0, sf::Vector2f(0.f, 0.f));
    polygon->setPoint(1, sf::Vector2f(50.f, 100.f));
    polygon->setPoint(2, sf::Vector2f(100.f, 0.f));
    polygon->setPoint(3, sf::Vector2f(75.f, -50.f));
    polygon->setPoint(4, sf::Vector2f(-25.f, -50.f));
    polygon->setFillColor(sf::Color::White);
    m_shape = polygon;
    return polygon;
  }
  default:
    break;
  }

  return nullptr;
}

/**
 * @brief Sets the position of the shape.
 *
 * @param x X coordinate.
 * @param y Y coordinate.
 */
void 
CShape::setPosition(float x, float y) {
  if (m_shape) {
    m_shape->setPosition(x, y);
  }
  else {
    ERROR("CShape", "setPosition", "Shape is not initialized.");
  }
}

/**
 * @brief Sets the position of the shape using a vector.
 *
 * @param position The position as a 2D vector.
 */
void 
CShape::setPosition(const sf::Vector2f& position) {
  if (m_shape) {
    m_shape->setPosition(position);
  }
  else {
    ERROR("CShape", "setPosition", "Shape is not initialized.");
  }
}

/**
 * @brief Sets the fill color of the shape.
 *
 * @param color The color to apply.
 */
void 
CShape::setFillColor(const sf::Color& color) {
  if (m_shape) {
    m_shape->setFillColor(color);
  }
  else {
    ERROR("CShape", "setFillColor", "Shape is not initialized.");
  }
}

/**
 * @brief Sets the rotation angle of the shape.
 *
 * @param angle The rotation angle in degrees.
 */
void 
CShape::SetRotation(float angle) {
  if (m_shape) {
    m_shape->setRotation(angle);
  }
  else {
    ERROR("CShape", "setRotation", "Shape is not initialized.");
  }
}

/**
 * @brief Sets the scale of the shape.
 *
 * @param scale The scaling factor as a 2D vector.
 */
void 
CShape::setScale(const sf::Vector2f& scale) {
  if (m_shape) {
    m_shape->setScale(scale);
  }
  else {
    ERROR("CShape", "setScale", "Shape is not initialized.");
  }
}
