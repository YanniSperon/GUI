#include "Texture.h"
#include "Console.h"
#include "TextureManager.h"

#include "stb_image.h"

Texture::Texture(const std::string& name, Type type)
	: m_ID(0), m_FakeUser(false), m_Path(name)
{
	LoadTexture(name, type);
}

Texture::Texture(const glm::vec4& color, Type type)
	: m_ID(0), m_FakeUser(false), m_Path("COLOR")
{
	LoadColor(color, type);
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_ID);
	Console::Warning("Deleted texture");
}

void Texture::LoadTexture(const std::string& name, Type type)
{
	glGenTextures(1, &m_ID);
	stbi_set_flip_vertically_on_load(1);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(name.c_str(), &width, &height, &nrChannels, 0);


	if (data)
	{
		GLenum format = GL_RED;
		switch (nrChannels)
		{
		case 1:
			format = GL_RED;
			break;
		case 3:
			format = GL_RGB;
			break;
		case 4:
			format = GL_RGBA;
			break;
		default:
			break;
		}

		glBindTexture(GL_TEXTURE_2D, m_ID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, GL_UNSIGNED_BYTE, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		switch (type)
		{
		case Texture::Type::TWOD:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			break;
		case Texture::Type::THREED:
			glGenerateMipmap(GL_TEXTURE_2D);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			break;
		default:
			break;
		}


		stbi_image_free(data);
	}
	else
	{
		stbi_image_free(data);
		Console::Error("Failed to load texture \"%s\"", name.c_str());
	}
}

void Texture::LoadColor(const glm::vec4& color, Type type)
{
	GLubyte colorImageData[] = {
		(GLubyte)color.x * 255,
		(GLubyte)color.y * 255,
		(GLubyte)color.z * 255,
		(GLubyte)color.w * 255
	};
	
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(1, &m_ID);
	glBindTexture(GL_TEXTURE_2D, m_ID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
		GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, colorImageData);
}

void Texture::Bind(int slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_ID);
}

GLuint Texture::GetID()
{
	return m_ID;
}

void Texture::SetHasFakeUser(bool fakeUser)
{
	m_FakeUser = fakeUser;
	if (!fakeUser) {
		TextureManager::GetInstance()->MarkForCleanup();
	}
}

bool Texture::GetHasFakeUser()
{
	return m_FakeUser;
}

std::string Texture::GetPath()
{
	return m_Path;
}

void Texture::Unbind(int slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, 0);
}
