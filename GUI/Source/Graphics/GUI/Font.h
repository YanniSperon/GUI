#pragma once

#include "Shader.h"

#include <glew.h>
#include <glm.hpp>
#include <string>
#include <map>

namespace GUI {

	struct Character {
		glm::vec2 bottomLeftTexCoord; // Texture coordinates of the bottom left of the rect
		glm::vec2 topRightTexCoord;   // Texture coordinates of the top right of the rect
		glm::ivec2 size;              // Size of glyph
		glm::ivec2 bearing;           // Offset from baseline to left/top of glyph
		glm::ivec2 advance;           // Offset to advance to next glyph
		GLfloat vertices[24];         // Vertex data including the size and the vertex coordinates
		std::map<char, glm::ivec2> kerning;          // Specific offsets for following characters
	};

	class Font {
	private:
		std::map<char, Character> m_Characters;

		GLuint m_AtlasTextureID;

		unsigned int m_AtlasHeight;
		unsigned int m_AtlasWidth;
		
		std::string m_Path;

		unsigned int m_Size;
		unsigned int m_PtSize;

		//GLuint m_VAO;
		//GLuint m_VBO;
		//
		//Shader* tempShader;

		bool m_HasFakeUser;

	public:

		Font(const std::string& path, unsigned int size, bool usePtSize);
		~Font();

		void Draw(const glm::mat4& projection);

		const std::string& GetPath();
		unsigned int GetSize();

		std::map<char, Character>& GetCharacters();
		GLuint GetAtlasTextureID();

		void SetHasFakeUser(bool hasFakeUser);
		bool GetHasFakeUser();
	};
}