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
		signed long advance;         // Offset to advance to next glyph
		GLfloat vertices[24];         // Vertex data including the size and the vertex coordinates
	};

	class Font {
	private:
		std::map<char, Character> m_Characters;

		GLuint m_AtlasTextureID;
		
		std::string m_Path;

		unsigned int m_Size;

		GLuint m_VAO;
		GLuint m_VBO;

		Shader* tempShader;

		glm::vec4 m_TintColor;
		float m_Opacity;

	public:

		Font(const std::string& path, unsigned int size);
		~Font();

		void Draw(const glm::mat4& projection);

		std::map<char, Character>& GetCharacters();
		GLuint GetAtlasTextureID();
	};
}