#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

namespace GUI {
	class FreetypeManager {
	private:
		FT_Library m_Library;
	public:
		FreetypeManager();
		~FreetypeManager();

		FT_Library& GetLibrary();

		static void InitializeSingleton();
		static FreetypeManager* GetInstance();
		static void CleanupSingleton();
	};
}