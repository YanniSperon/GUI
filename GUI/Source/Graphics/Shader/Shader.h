#pragma once

#include <glew.h>
#include <string>

#include "Vendor/glm/glm.hpp"

#define SHADER_VERTEX_SHADER 1
#define SHADER_FRAGMENT_SHADER 2
#define SHADER_GEOMETRY_SHADER 4

class Shader {
private:
	GLuint m_ID;
	//GLuint m_UBO;

	bool m_FakeUser;

	std::string m_Path;

	int m_ShaderType;
public:
	Shader(const std::string& name, int shaderType);
	~Shader();

	GLuint LoadShader(const char* path, GLenum type);

	void Bind(/*const glm::mat4& projection, const glm::mat4& view*/);

	void SetBool(const std::string& name, bool value);
	void SetInt(const std::string& name, int value);

	void SetFloat(const std::string& name, float value);
	void Set2Float(const std::string& name, float x, float y);
	void Set3Float(const std::string& name, float x, float y, float z);
	void Set4Float(const std::string& name, float x, float y, float z, float w);

	void SetVec2(const std::string& name, const glm::vec2& value);
	void SetVec3(const std::string& name, const glm::vec3& value);
	void SetVec4(const std::string& name, const glm::vec4& value);

	void SetMat2(const std::string& name, const glm::mat2& value);
	void SetMat3(const std::string& name, const glm::mat3& value);
	void SetMat4(const std::string& name, const glm::mat4& value);

	GLuint GetID();

	void SetHasFakeUser(bool fakeUser);
	bool GetHasFakeUser();
	int GetShaderType();

	std::string GetPath();

	static void Unbind();
};
