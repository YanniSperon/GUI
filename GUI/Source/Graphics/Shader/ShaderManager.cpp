#include "ShaderManager.h"
#include "Console.h"


static ShaderManager* s_ShaderManager;

ShaderManager::ShaderManager()
{
	m_Shaders = std::unordered_map<std::string, std::shared_ptr<Shader>>();
}

ShaderManager::~ShaderManager()
{
	m_Shaders.clear();
}

std::shared_ptr<Shader> ShaderManager::GetShader(const std::string& name, int shaderType)
{
	if (m_Shaders.find(name) != m_Shaders.end()) {
		return m_Shaders[name];
	}
	else {
		m_Shaders[name] = std::make_shared<Shader>(name, shaderType);
		return m_Shaders[name];
	}
}

std::unordered_map<std::string, std::shared_ptr<Shader>>& ShaderManager::GetManagedShaders()
{
	return m_Shaders;
}

void ShaderManager::Cleanup()
{
	if (m_ShouldCleanup) {
		Shader::Unbind();
		auto it = m_Shaders.begin();
		while (it != m_Shaders.end()) {
			if (it->second.unique() && !it->second->GetHasFakeUser()) {
				it = m_Shaders.erase(it);
			}
			else {
				it++;
			}
		}
		m_ShouldCleanup = false;
	}
}

void ShaderManager::MarkForCleanup()
{
	m_ShouldCleanup = true;
}

void ShaderManager::InitializeSingleton()
{
	s_ShaderManager = new ShaderManager();
}

ShaderManager* ShaderManager::GetInstance()
{
	return s_ShaderManager;
}

void ShaderManager::CleanupSingleton()
{
	delete s_ShaderManager;
}
