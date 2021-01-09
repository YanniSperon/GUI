#pragma once

#include <glew.h>
#include <glm.hpp>
#include <string>

class Texture {
private:
	GLuint m_ID;

	bool m_FakeUser;

	std::string m_Path;

public:
	enum class Type {
		TWOD = 0,
		THREED = 1,
	};
	Texture(const std::string& name, Type type);
	Texture(const glm::vec4& color, Type type);
	~Texture();

	void LoadTexture(const std::string& name, Type type);
	void LoadColor(const glm::vec4& color, Type type);

	void Bind(int slot);

	GLuint GetID();

	void SetHasFakeUser(bool fakeUser);
	bool GetHasFakeUser();

	std::string GetPath();

	static void Unbind(int slot);
};
