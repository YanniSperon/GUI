#pragma once

#include "Shader.h"

#include <string>
#include <memory>
#include <unordered_map>

class ShaderManager {
private:
	std::unordered_map<std::string, std::shared_ptr<Shader>> m_Shaders;

	bool m_ShouldCleanup;
public:
	ShaderManager();
	~ShaderManager();

	std::shared_ptr<Shader> GetShader(const std::string& name, int shaderType);

	std::unordered_map<std::string, std::shared_ptr<Shader>>& GetManagedShaders();

	void Cleanup();

	void MarkForCleanup();

	static void InitializeSingleton();
	static ShaderManager* GetInstance();
	static void CleanupSingleton();
};
