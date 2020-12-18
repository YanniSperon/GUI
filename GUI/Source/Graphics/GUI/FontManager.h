#pragma once

#include "Font.h"

#include <string>
#include <unordered_map>

namespace GUI {
	class FontManager {
	private:
		std::unordered_map<std::string, Font*> m_Fonts;
	public:
		FontManager();
		~FontManager();

		Font* GetFont(const std::string& path, unsigned int size);
		std::unordered_map<std::string, Font*>& GetManagedFonts();

		static void InitializeSingleton();
		static FontManager* GetInstance();
		static void CleanupSingleton();
	};
}