#pragma once

#include "Texture.h"

#include <string>
#include <memory>
#include <unordered_map>

class TextureManager {
private:
	std::unordered_map<std::string, std::shared_ptr<Texture>> m_Textures;

	bool m_ShouldCleanup;
public:
	TextureManager();
	~TextureManager();

	std::shared_ptr<Texture> GetTexture(const std::string& name);

	std::unordered_map<std::string, std::shared_ptr<Texture>>& GetManagedTextures();

	void Cleanup();

	void MarkForCleanup();

	static void InitializeSingleton();
	static TextureManager* GetInstance();
	static void CleanupSingleton();
};
