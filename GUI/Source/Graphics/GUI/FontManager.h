#pragma once

#include "Font.h"

#include <memory>
#include <string>
#include <unordered_map>

namespace GUI {
	class FontManager {
	private:
		std::unordered_map<std::string, std::shared_ptr<Font>> m_Fonts;

		bool m_ShouldCleanup;
	public:
		FontManager();
		~FontManager();

		std::shared_ptr<Font> GetFont(const std::string& path, unsigned int size, unsigned int dpi);
		std::unordered_map<std::string, std::shared_ptr<Font>>& GetManagedFonts();

		void Cleanup();

		void MarkForCleanup();

		static void InitializeSingleton();
		static FontManager* GetInstance();
		static void CleanupSingleton();
	};
}