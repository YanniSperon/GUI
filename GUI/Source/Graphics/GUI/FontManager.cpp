#include "FontManager.h"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace GUI {

	static FontManager* s_FontManager;

	FontManager::FontManager()
		: m_Fonts(), m_ShouldCleanup(false)
	{

	}

	FontManager::~FontManager()
	{
		m_Fonts.clear();
	}

	std::shared_ptr<Font> FontManager::GetFont(const std::string& path, unsigned int size, bool usePtSize)
	{
		std::string name = path + std::to_string(size) + std::to_string(usePtSize);
		if (m_Fonts.find(name) != m_Fonts.end()) {
			return m_Fonts[name];
		}
		else {
			m_Fonts[name] = std::make_shared<Font>(path, size, usePtSize);
			return m_Fonts[name];
		}
	}

	std::unordered_map<std::string, std::shared_ptr<Font>>& FontManager::GetManagedFonts()
	{
		return m_Fonts;
	}

	void FontManager::Cleanup()
	{
		if (m_ShouldCleanup) {
			Shader::Unbind();
			auto it = m_Fonts.begin();
			while (it != m_Fonts.end()) {
				if (it->second.unique() && !it->second->GetHasFakeUser()) {
					it = m_Fonts.erase(it);
				}
				else {
					it++;
				}
			}
			m_ShouldCleanup = false;
		}
	}

	void FontManager::MarkForCleanup()
	{
		m_ShouldCleanup = true;
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