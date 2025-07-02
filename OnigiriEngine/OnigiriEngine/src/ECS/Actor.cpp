#include "ECS/Actor.h"

Actor::Actor(const std::string& actorName) {
  //Setup Actor Name
  m_name = actorName;

  //Setup Transform
  EngineUtilities::TSharedPointer<CShape> shape = EngineUtilities::MakeShared<CShape>();
  addComponent(shape);

  //setup Shape
  EngineUtilities::TSharedPointer<Transform> transform = EngineUtilities::MakeShared<Transform>();
  addComponent(transform);
}

void Actor::start() {
  // Inicializa el actor aquí si es necesario
}

void Actor::destroy() {
  // Libera recursos si es necesario
}


 void
 Actor::update(float deltaTime) {
    auto transform = getComponent<Transform>();
    auto shape = getComponent<CShape>();

    if (transform && shape) {
      shape->setPosition(transform->getPosition());
      shape->setRotation(transform->getRotation().x);
      shape->setScale(transform->getScale());
    }
  }


void
Actor::render(const EngineUtilities::TSharedPointer<Window>& window) {
  for (unsigned int i = 0; i < components.size(); i++) {
    auto shape = components[i].dynamic_pointer_cast<CShape>();
    if(shape) {
      shape->render(window);
    }
  }
}

