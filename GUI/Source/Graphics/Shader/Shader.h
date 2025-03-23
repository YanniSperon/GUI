#pragma once

#include <glew.h>
#include <string>

#include "Vendor/glm/glm.hpp"

#define SHADER_TYPE_VERTEX                  1u << 0
#define SHADER_TYPE_FRAGMENT                1u << 1
#define SHADER_TYPE_GEOMETRY                1u << 2
#define SHADER_TYPE_TESSELLATION_CONTROL    1u << 3
#define SHADER_TYPE_TESSELLATION_EVALUATION 1u << 4
#define SHADER_TYPE_COMPUTE                 1u << 5

#define SHADER_TYPE_DEFAULT                 SHADER_TYPE_VERTEX | SHADER_TYPE_FRAGMENT
#define SHADER_TYPE_ALL                     SHADER_TYPE_VERTEX | SHADER_TYPE_FRAGMENT | SHADER_TYPE_GEOMETRY | SHADER_TYPE_TESSELLATION_CONTROL | SHADER_TYPE_TESSELLATION_EVALUATION | SHADER_TYPE_COMPUTE
#define SHADER_TYPE_TESSELLATION            SHADER_TYPE_TESSELLATION_CONTROL | SHADER_TYPE_TESSELLATION_EVALUATION

class Shader {
public:
	enum class Type: unsigned int {
		Vertex = SHADER_TYPE_VERTEX,
		Fragment = SHADER_TYPE_FRAGMENT,
		Geometry = SHADER_TYPE_GEOMETRY,
		TessellationControl = SHADER_TYPE_TESSELLATION_CONTROL,
		TessellationEvaluation = SHADER_TYPE_TESSELLATION_EVALUATION,
		COMPUTE = SHADER_TYPE_COMPUTE,

		Default = SHADER_TYPE_DEFAULT,
		All = SHADER_TYPE_ALL,
		Tessellation = SHADER_TYPE_TESSELLATION,
	};

private:
	GLuint m_ID;
	//GLuint m_UBO;

	bool m_FakeUser;

	std::string m_Path;

	Type m_ShaderType;
public:
	Shader(const std::string& name, Type shaderType);
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
	Type GetShaderType();

	const std::string& GetPath();

	static void Unbind();
};