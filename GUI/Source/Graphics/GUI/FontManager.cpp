#include "FontManager.h"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace GUI {

	static FontManager* s_FontManager;

	FontManager::FontManager()
		: m_Fonts()
	{

	}

	FontManager::~FontManager()
	{
		for (const auto& font : m_Fonts)
		{
			delete font.second;
		}
	}

	Font* FontManager::GetFont(const std::string& path, int size)
	{
		std::string name = path + std::to_string(size);
		if (m_Fonts.find(name) != m_Fonts.end()) {
			return m_Fonts[name];
		}
		else {
			m_Fonts[name] = new Font(path, size);
			return m_Fonts[name];
		}
	}

	std::unordered_map<std::string, Font*>& FontManager::GetManagedFonts()
	{
		return m_Fonts;
	}

	void FontManager::InitializeSingleton()
	{
		s_FontManager = new FontManager();
	}

	FontManager* FontManager::GetInstance()
	{
		return s_FontManager;
	}

	void FontManager::CleanupSingleton()
	{
		delete s_FontManager;
	}
}