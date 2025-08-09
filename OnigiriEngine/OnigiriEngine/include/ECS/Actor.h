#pragma once
#include "..//Prerequisites.h"
#include "Entity.h"
#include "CShape.h"
#include "Transform.h"
#include "ECS/Texture.h"

/**
 * @class Actor
 * @brief Representa una entidad activa del mundo del juego que puede tener componentes, ser actualizada, renderizada y destruida.
 * Hereda de Entity.
 */
class Actor : public Entity {
public:

  Actor() = default;
  Actor(const std::string& actorName);

  virtual ~Actor() = default;

  void 
  start() override;

  void 
  update(float deltaTime) override;

  void 
  render(const EngineUtilities::TSharedPointer<Window>& window) override;

  void 
  destroy() override;

  void 
  setTexture(const EngineUtilities::TSharedPointer<Texture>& texture);

  // ⬇️ NUEVO: getter para el nombre (usado en el ranking/ImGui)
  const std::string& getName() const { return m_name; }

  template <typename T>
  EngineUtilities::TSharedPointer<T> getComponent();

private:
  std::string m_name = "Actor";
};

template <typename T>
inline EngineUtilities::TSharedPointer<T> Actor::getComponent() {
  for (auto& component : components) {
    EngineUtilities::TSharedPointer<T> specificComponent = component.template dynamic_pointer_cast<T>();
    if (specificComponent) {
      return specificComponent;
    }
  }
  return EngineUtilities::TSharedPointer<T>();
}
