#include "Global.h"
#include "Console.h"

#include "TextureManager.h"
#include "ShaderManager.h"
#include "FreetypeManager.h"
#include "FontManager.h"

#include <memory>

static std::vector<std::string> g_ConsoleLog;
static std::chrono::steady_clock::time_point g_SystemStartTime;

void Global::Initialize()
{
	g_ConsoleLog = std::vector<std::string>();
	g_SystemStartTime = std::chrono::high_resolution_clock::now();

	TextureManager::InitializeSingleton();
	ShaderManager::InitializeSingleton();
	GUI::FreetypeManager::InitializeSingleton();
	GUI::FontManager::InitializeSingleton();
}

std::chrono::steady_clock::time_point& Global::GetStartTime()
{
	return g_SystemStartTime;
}

std::vector<std::string>& Global::GetConsoleLog()
{
	return g_ConsoleLog;
}

void Global::Update()
{
	TextureManager::GetInstance()->Cleanup();
	ShaderManager::GetInstance()->Cleanup();
	GUI::FontManager::GetInstance()->Cleanup();
}

void Global::Cleanup()
{
	TextureManager::CleanupSingleton();
	ShaderManager::CleanupSingleton();
	GUI::FontManager::CleanupSingleton();
	GUI::FreetypeManager::CleanupSingleton();
}
