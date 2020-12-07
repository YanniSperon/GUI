#include "TextureManager.h"
#include "Console.h"


static TextureManager* s_TextureManager;

TextureManager::TextureManager()
{
	m_Textures = std::unordered_map<std::string, std::shared_ptr<Texture>>();
}

TextureManager::~TextureManager()
{
	m_Textures.clear();
}

std::shared_ptr<Texture> TextureManager::GetTexture(const std::string& name)
{
	if (m_Textures.find(name) != m_Textures.end()) {
		return m_Textures[name];
	}
	else {
		m_Textures[name] = std::make_shared<Texture>(name);
		return m_Textures[name];
	}
}

std::unordered_map<std::string, std::shared_ptr<Texture>>& TextureManager::GetManagedTextures()
{
	return m_Textures;
}

void TextureManager::Cleanup()
{
	if (m_ShouldCleanup) {
		Texture::Unbind(0);
		auto it = m_Textures.begin();
		while (it != m_Textures.end()) {
			if (it->second.unique() && !it->second->GetHasFakeUser()) {
				it = m_Textures.erase(it);
			}
			else {
				it++;
			}
		}
		m_ShouldCleanup = false;
	}
}

void TextureManager::MarkForCleanup()
{
	m_ShouldCleanup = true;
}

void TextureManager::InitializeSingleton()
{
	s_TextureManager = new TextureManager();
}

TextureManager* TextureManager::GetInstance()
{
	return s_TextureManager;
}

void TextureManager::CleanupSingleton()
{
	delete s_TextureManager;
}
