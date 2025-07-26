#pragma once
#include "Prerequisites.h"
#include "ECS/Texture.h"

class
ResourceManager {
private:
  ResourceManager() = default;
  ~ResourceManager() = default;


public:
  ResourceManager(const ResourceManager&) = default;
  ResourceManager& operator =(const ResourceManager&) = delete;

  static ResourceManager& 
  getInstance() {
    static ResourceManager instance;
      return instance;
  }

  bool
  loadTexture(const std::string& fileName, const std::string& extension);

  EngineUtilities::TSharedPointer<Texture>
  getTexture(const std::string& fileName);


private:
  std::unordered_map<std::string, EngineUtilities::TSharedPointer<Texture>> m_texture;
};