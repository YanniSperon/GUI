#include "FreetypeManager.h"
#include "Console.h"

namespace GUI {
	static FreetypeManager* s_FreetypeManager;

	FreetypeManager::FreetypeManager()
	{
		if (FT_Init_FreeType(&m_Library))
		{
			Console::Error("Could not initialize FreeType Library");
		}
	}

	FreetypeManager::~FreetypeManager()
	{
		FT_Done_FreeType(m_Library);
	}

	FT_Library& FreetypeManager::GetLibrary()
	{
		return m_Library;
	}

	void FreetypeManager::InitializeSingleton()
	{
		s_FreetypeManager = new FreetypeManager();
	}

	FreetypeManager* FreetypeManager::GetInstance()
	{
		return s_FreetypeManager;
	}

	void FreetypeManager::CleanupSingleton()
	{
		delete s_FreetypeManager;
	}
}