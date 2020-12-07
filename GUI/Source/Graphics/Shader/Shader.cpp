#include "Shader.h"
#include "Console.h"
#include "Global.h"
#include "ShaderManager.h"

#include <string>
#include <fstream>
#include <sstream>

Shader::Shader(const std::string& name, int shaderType)
	: m_ID(0), m_FakeUser(false), m_Path(name), m_ShaderType(shaderType)
{
	m_ID = glCreateProgram();

	int vertID, fragID, geomID, tescID;
	if (shaderType & SHADER_VERTEX_SHADER) {
		std::string vertexShaderString = name + ".vert";
		vertID = LoadShader(vertexShaderString.c_str(), GL_VERTEX_SHADER);
	}
	if (shaderType & SHADER_FRAGMENT_SHADER) {
		std::string fragmentShaderString = name + ".frag";
		fragID = LoadShader(fragmentShaderString.c_str(), GL_FRAGMENT_SHADER);
	}
	if (shaderType & SHADER_GEOMETRY_SHADER) {
		std::string geometryShaderString = name + ".geom";
		geomID = LoadShader(geometryShaderString.c_str(), GL_GEOMETRY_SHADER);
	}


	int success;
	char infoLog[512];
	glLinkProgram(m_ID);
	glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(m_ID, 512, NULL, infoLog);
		Console::Error("Shader type: %i at path \"%s\" linking failed \"%s\"", shaderType, name.c_str(), infoLog);
	}

	if (shaderType & SHADER_VERTEX_SHADER) {
		glDeleteShader(vertID);
	}
	if (shaderType & SHADER_FRAGMENT_SHADER) {
		glDeleteShader(fragID);
	}
	if (shaderType & SHADER_GEOMETRY_SHADER) {
		glDeleteShader(geomID);
	}

	//glUniformBlockBinding(m_ID, glGetUniformBlockIndex(m_ID, "ubo_Matrices"), 0);
	//
	//glGenBuffers(1, &m_UBO);
	//
	//glBindBuffer(GL_UNIFORM_BUFFER, m_UBO);
	//glBufferData(GL_UNIFORM_BUFFER, 128, nullptr, GL_DYNAMIC_DRAW);
	//glBindBufferRange(GL_UNIFORM_BUFFER, 0, m_UBO, 0, 2 * sizeof(glm::mat4));
}

Shader::~Shader()
{
	glDeleteProgram(m_ID);
	//glDeleteBuffers(1, &m_UBO);
	Console::Warning("Deleted shader");
}

GLuint Shader::LoadShader(const char* path, GLenum type)
{
	std::string shaderCode;
	std::ifstream shaderFile(path);
	
	if (shaderFile.is_open()) {
		std::stringstream shaderStream;
		shaderStream << shaderFile.rdbuf();
		shaderFile.close();
		shaderCode = shaderStream.str();
	}
	else {
		Console::Error("Shader type: %i at path \"%s\" invalid!", type, path);
	}
	const char* shaderCodeChar = shaderCode.c_str();

	unsigned int shaderID;
	int success;
	char infoLog[512];

	shaderID = glCreateShader(type);
	glShaderSource(shaderID, 1, &shaderCodeChar, NULL);
	glCompileShader(shaderID);
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
		Console::Error("Shader type: %i at path \"%s\" failed compilation!", type, path);
	};

	glAttachShader(m_ID, shaderID);

	return shaderID;
}

void Shader::Bind(/*const glm::mat4& projection, const glm::mat4& view*/)
{
	glUseProgram(m_ID);
	//glBindBuffer(GL_UNIFORM_BUFFER, m_UBO);
	//glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &projection[0][0]);
	//glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &view[0][0]);
}

void Shader::SetBool(const std::string& name, bool value)
{
	glUniform1i(glGetUniformLocation(m_ID, name.c_str()), (int)value);
}

void Shader::SetInt(const std::string& name, int value)
{
	glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, float value)
{
	glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
}

void Shader::Set2Float(const std::string& name, float x, float y)
{
	glUniform2f(glGetUniformLocation(m_ID, name.c_str()), x, y);
}

void Shader::Set3Float(const std::string& name, float x, float y, float z)
{
	glUniform3f(glGetUniformLocation(m_ID, name.c_str()), x, y, z);
}

void Shader::Set4Float(const std::string& name, float x, float y, float z, float w)
{
	glUniform4f(glGetUniformLocation(m_ID, name.c_str()), x, y, z, w);
}

void Shader::SetVec2(const std::string& name, const glm::vec2& value)
{
	glUniform2f(glGetUniformLocation(m_ID, name.c_str()), value.x, value.y);
}

void Shader::SetVec3(const std::string& name, const glm::vec3& value)
{
	glUniform3f(glGetUniformLocation(m_ID, name.c_str()), value.x, value.y, value.z);
}

void Shader::SetVec4(const std::string& name, const glm::vec4& value)
{
	glUniform4f(glGetUniformLocation(m_ID, name.c_str()), value.x, value.y, value.z, value.w);
}

void Shader::SetMat2(const std::string& name, const glm::mat2& value)
{
	glUniformMatrix2fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void Shader::SetMat3(const std::string& name, const glm::mat3& value)
{
	glUniformMatrix3fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void Shader::SetMat4(const std::string& name, const glm::mat4& value)
{
	glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

GLuint Shader::GetID()
{
	return m_ID;
}

void Shader::SetHasFakeUser(bool fakeUser)
{
	m_FakeUser = fakeUser;
	if (!fakeUser) {
		ShaderManager::GetInstance()->MarkForCleanup();
	}
}

bool Shader::GetHasFakeUser()
{
	return m_FakeUser;
}

int Shader::GetShaderType()
{
	return m_ShaderType;
}

std::string Shader::GetPath()
{
	return m_Path;
}

void Shader::Unbind()
{
	glUseProgram(0);
}
