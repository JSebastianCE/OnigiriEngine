#pragma once

#include "../Prerequisites.h"

/**
 * @file CVector2.h
 * @brief Represents a custom 2D vector with common mathematical operations.
 */
class CVector2 {
public:
  // Public members

  /**
   * @brief The X component of the vector.
   */
  float x;

  /**
   * @brief The Y component of the vector.
   */
  float y;

  // Constructors

  /**
   * @brief Default constructor. Initializes the vector to (0, 0).
   */
  CVector2() : x(0.f), y(0.f) {}

  /**
   * @brief Parameterized constructor.
   * @param x The X component.
   * @param y The Y component.
   */
  CVector2(float x, float y) : x(x), y(y) {}

  // === Arithmetic operators ===

  /**
   * @brief Adds two vectors.
   * @param other The vector to add.
   * @return The resulting vector after addition.
   */
  CVector2 
  operator+(const CVector2& other) const {
    return CVector2(x + other.x, y + other.y);
  }

  /**
   * @brief Subtracts a vector from this vector.
   * @param other The vector to subtract.
   * @return The resulting vector after subtraction.
   */
  CVector2 
  operator-(const CVector2& other) const {
    return CVector2(x - other.x, y - other.y);
  }
  
  /**
   * @brief Multiplies the vector by a scalar.
   * @param scalar The scalar value.
   * @return The resulting scaled vector.
   */
  CVector2 
  operator*(float scalar) const {
    return CVector2(x * scalar, y * scalar);
  }

  /**
   * @brief Divides the vector by a scalar.
   * @param divisor The scalar divisor.
   * @return The resulting scaled vector.
   */
  CVector2 
  operator/(float divisor) const {
    return CVector2(x / divisor, y / divisor);
  }

  // === Compound assignment operators ===

  /**
   * @brief Adds another vector to this vector (in-place).
   * @param other The vector to add.
   * @return Reference to this vector after addition.
   */
  CVector2& 
  operator+=(const CVector2& other) {
    x += other.x;
    y += other.y;
    return *this;
  }

  /**
   * @brief Subtracts another vector from this vector (in-place).
   * @param other The vector to subtract.
   * @return Reference to this vector after subtraction.
   */
  CVector2& 
  operator-=(const CVector2& other) {
    x -= other.x;
    y -= other.y;
    return *this;
  }

  /**
   * @brief Multiplies this vector by a scalar (in-place).
   * @param scalar The scalar value.
   * @return Reference to this vector after scaling.
   */
  CVector2& 
  operator*=(float scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
  }

  /**
   * @brief Divides this vector by a scalar (in-place).
   * @param scalar The scalar divisor.
   * @return Reference to this vector after scaling.
   */
  CVector2& 
  operator/=(float scalar) {
    x /= scalar;
    y /= scalar;
    return *this;
  }

  //Comparison operators

  /**
   * @brief Checks if two vectors are equal.
   * @param other The vector to compare with.
   * @return True if both vectors have the same components, false otherwise.
   */
  bool 
  operator==(const CVector2& other) const {
    return x == other.x && y == other.y;
  }

  /**
   * @brief Checks if two vectors are not equal.
   * @param other The vector to compare with.
   * @return True if vectors differ in any component, false otherwise.
   */
  bool 
  operator!=(const CVector2& other) const {
    return !(*this == other);
  }
};
