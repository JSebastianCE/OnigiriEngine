#pragma once

/**
 * @file CShape.h
 * @brief Defines the CShape class for creating and managing SFML shapes.
 */

#include "Prerequisites.h"
#include "Memory/TUniquePtr.h"
#include <CShape.h>
#include <Memory/TSharedPointer.h>
#include "ECS\Component.h"

class
  Window;

/**
 * @class CShape
 * @brief A class for handling 2D shapes in a graphical application using SFML.
 */
class
  CShape : public Component{
public:
  /**
   * @brief Default constructor.
   */
  CShape() = default;


  CShape(ShapeType shapeType) : m_shapePtr(nullptr),
                                m_shapeType(ShapeType::EMPTY),
                                Component(ComponentType::SHAPE) {
  }








  /**
   * @brief Constructor that initializes the shape type.
   * @param shapeType Type of shape to initialize.
   */
  CShape(ShapeType shapeType)
    : m_shapePtr(nullptr), m_shapeType(ShapeType::EMPTY) {
  }

  /**
   * @brief Default destructor.
   */
  virtual
  ~CShape() = default;

  /**
   * @brief Creates a shape based on the specified ShapeType.
   * @param shapeType Enum value of the shape to create.
   */
  void createShape(ShapeType shapeType); // Cambiado el tipo de retorno a void

  /**
   * @brief Updates the shape (currently unused but available for future use).
   * @param deltaTime Time elapsed since last frame.
   */
  void
    update(float deltaTime);

  /**
   * @brief Renders the shape to the specified window.
   * @param window Reference to the custom Window object.
   */
  void
    render(const EngineUtilities::TSharedPointer<Window>& window); // Adaptado para usar smart pointer

  /**
   * @brief Sets the position of the shape.
   * @param x X coordinate.
   * @param y Y coordinate.
   */
  void
    setPosition(float x, float y);

  /**
   * @brief Sets the position of the shape using a vector.
   * @param position 2D vector specifying the position.
   */
  void
    setPosition(const sf::Vector2f& position);

  /**
   * @brief Sets the fill color of the shape.
   * @param color SFML color to fill the shape with.
   */
  void
    setFillColor(const sf::Color& color);

  /**
   * @brief Sets the rotation of the shape.
   * @param angle Rotation angle in degrees.
   */
  void
    SetRotation(float angle);

  /**
   * @brief Sets the scale of the shape.
   * @param scl Scale factor as a 2D vector.
   */
  void
    setScale(const sf::Vector2f& scl);

  /**
   * @brief Returns the raw SFML shape pointer.
   * @return Pointer to the current SFML shape.
   */
  sf::Shape* getShape() { return m_shapePtr ? m_shapePtr.operator->() : nullptr; }

private:
  EngineUtilities::TSharedPointer<sf::Shape> m_shapePtr; ///< Smart pointer to the SFML shape.
  ShapeType m_shapeType = ShapeType::EMPTY;             ///< Current type of shape.
  sf::VertexArray* m_line = nullptr;                    ///< Reserved for line shapes or outlines (not used yet).
};
