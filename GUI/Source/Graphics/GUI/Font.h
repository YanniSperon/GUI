#pragma once

#include "Shader.h"

#include <glew.h>
#include <glm.hpp>
#include <string>
#include <map>

namespace GUI {
	class Font {
	private:
		struct Character {
			glm::vec2 bottomLeftTexCoord; // Texture coordinates of the bottom left of the rect
			glm::vec2 topRightTexCoord;   // Texture coordinates of the top right of the rect
			glm::ivec2 size;              // Size of glyph
			glm::ivec2 bearing;           // Offset from baseline to left/top of glyph
			unsigned int advance;         // Offset to advance to next glyph
		};

		std::map<char, Character> m_CharacterTextureCoordinates;

		GLuint m_TextureAtlasID;
		
		std::string m_Path;

		int m_Size;

		Shader* tempShader;

		GLuint m_VAO;
		GLuint m_VBO;

	public:
		Font(const std::string& path, int size);
		~Font();

		void Draw();
	};
}