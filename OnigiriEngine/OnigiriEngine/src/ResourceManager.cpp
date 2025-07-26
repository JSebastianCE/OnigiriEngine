#include "ResourceManager.h"

bool
ResourceManager::loadTexture(const std::string& fileName,
  const std::string& extension) {

  //Verificar si la textura esta cargada
  if (m_texture.find(fileName) != m_texture.end())
    return true;

  //Crear y cargar la textura
  auto texture = EngineUtilities::MakeShared<Texture>(fileName, extension);
  m_texture[fileName] = texture;
  return true;
}

EngineUtilities::TSharedPointer<Texture> 
ResourceManager::getTexture(const std::string& fileName) {
  //Buscar la textura
  auto it = m_texture.find(fileName);
  if (it != m_texture.end()) {
    return it->second;
  }

  //Mensaje de advertencia
  std::cerr << "[ResourceManager] Texture not found: " << fileName << ". Using defaul texture .\n";

  const std::string defaultKey = "Default";

  //Verificar si la textura por defecto y aesta cargada
  auto defaultIt = m_texture.find(defaultKey);
  if (defaultIt != m_texture.end()) {
    return defaultIt->second;
  }

  //Cargar textura por defecto
  auto defaultTexture = EngineUtilities::MakeShared<Texture>(defaultKey, "png");
  m_texture[defaultKey] = defaultTexture;
  return defaultTexture;


}
