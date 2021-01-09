#include "Font.h"
#include "Console.h"
#include "FreetypeManager.h"

#include <glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

#define PIXEL_GAP 5

namespace GUI {
	Font::Font(const std::string& path, unsigned int size)
		: m_Path(path), m_Characters(), m_Size(size), m_HasFakeUser(false), m_AtlasHeight(size * 20), m_AtlasWidth(size * 20) //, tempShader(new Shader("Resources/Shaders/Text", SHADER_FRAGMENT_SHADER | SHADER_VERTEX_SHADER))
	{
		glGenTextures(1, &m_AtlasTextureID);

		glBindTexture(GL_TEXTURE_2D, m_AtlasTextureID);

		float inverseAtlasWidth = (1.0f / (float)m_AtlasWidth);
		float inverseAtlasHeight = (1.0f / (float)m_AtlasHeight);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_AtlasWidth, m_AtlasHeight, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		FT_Face face;
		if (FT_New_Face(FreetypeManager::GetInstance()->GetLibrary(), path.c_str(), 0, &face))
		{
			Console::Error("Failed to load font \"%s\"", path.c_str());
		}
		else {
			FT_Set_Pixel_Sizes(face, 0, m_Size);

			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			int xOffset = 0;
			int yOffset = 0;

			for (unsigned char c = 0; c < 255; c++)
			{
				if (FT_Load_Char(face, c, FT_LOAD_RENDER))
				//if (FT_Load_Char(face, c, FT_LOAD_NO_HINTING | FT_LOAD_MONOCHROME)) This produces a monochrome image, need to somehow convert monochrome to GL_RED or something
				{
					Console::Error("Failed to load Glyph");
					continue;
				}

				int width = face->glyph->bitmap.width;
				int height = face->glyph->bitmap.rows;

				int floatXOffset = (float)xOffset;
				int floatYOffset = (float)yOffset;

				float floatWidth = (float)width;
				float floatHeight = (float)height;

				glm::ivec2 advance = glm::ivec2(face->glyph->advance.x, face->glyph->advance.y);

				glm::ivec2 charSize = glm::ivec2(width, height);
				glm::vec2 charSizeFloat = glm::vec2(charSize);
				glm::ivec2 bearing = glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top);
				glm::vec2 bearingFloat = glm::vec2(bearing.x, bearing.y);

				float floatHalfWidth = floatWidth * 0.5f;
				float floatHalfHeight = floatHeight * 0.5f;

				glm::vec2 bottomLeftTexCoord = glm::vec2(floatXOffset * inverseAtlasWidth, (floatYOffset + floatHeight) * inverseAtlasHeight);
				glm::vec2 topRightTexCoord = glm::vec2((floatXOffset + floatWidth) * inverseAtlasWidth, floatYOffset * inverseAtlasHeight);

				m_Characters[c] = {
					bottomLeftTexCoord,
					topRightTexCoord,
					charSize,
					bearing,
					advance,
					{
						0.0f, bearingFloat.y - charSizeFloat.y,             // Bottom Left
						bottomLeftTexCoord.x , bottomLeftTexCoord.y,        // 
						0.0f, bearingFloat.y,								// Top Left
						bottomLeftTexCoord.x, topRightTexCoord.y,			// 
						charSizeFloat.x, bearingFloat.y,					// Top Right
						topRightTexCoord.x, topRightTexCoord.y,				// 

						0.0f, bearingFloat.y - charSizeFloat.y,             // Bottom Left
						bottomLeftTexCoord.x, bottomLeftTexCoord.y,			// 
						charSizeFloat.x, bearingFloat.y,					// Top Right
						topRightTexCoord.x, topRightTexCoord.y,				// 
						charSizeFloat.x, bearingFloat.y - charSizeFloat.y,  // Bottom Right
						topRightTexCoord.x, bottomLeftTexCoord.y			// 
					},
					{}
				};

				if (FT_HAS_KERNING(face)) {
					for (unsigned char kChar = 0; kChar < 255; kChar++) {
						FT_Vector kerning;
						int error = FT_Get_Kerning(face, FT_Get_Char_Index(face, c), FT_Get_Char_Index(face, kChar), FT_KERNING_DEFAULT, &kerning);
						if (error) {
							Console::Error("Error loading character \'%c\' kerning for character \'%c\'", kChar, c);
							continue;
						}
						else if (!(kerning.x == 0 && kerning.y == 0)) {
							m_Characters[c].kerning[kChar] = glm::ivec2(kerning.x, kerning.y);
						}
					}
				}

				glTexSubImage2D(GL_TEXTURE_2D, 0, xOffset, yOffset, width, height, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

				xOffset += width + PIXEL_GAP;
				if (xOffset + size >= m_AtlasWidth) {
					xOffset = 0;
					yOffset += size + PIXEL_GAP;
					if (yOffset >= m_AtlasHeight) {
						break;
					}
				}
			}
			FT_Done_Face(face);
		}

		//GLfloat vertices[24] = {
		//	-0.5f, -0.5f, // Bottom Left
		//	0.0f, 1.0f,   // 
		//	-0.5f, 0.5f,  // Top Left
		//	0.0f, 0.0f,   // 
		//	0.5f, 0.5f,   // Top Right
		//	1.0f, 0.0f,   // 
		//
		//	-0.5f, -0.5f, // Bottom Left
		//	0.0f, 1.0f,   // 
		//	0.5f, 0.5f,   // Top Right
		//	1.0f, 0.0f,   // 
		//	0.5f, -0.5f,  // Bottom Right
		//	1.0f, 1.0f    // 
		//};
		//
		//glGenVertexArrays(1, &m_VAO);
		//glBindVertexArray(m_VAO);
		//
		//glGenBuffers(1, &m_VBO);
		//glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		//
		//glEnableVertexAttribArray(0);
		//glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
		//glEnableVertexAttribArray(1);
		//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(sizeof(GLfloat) * 2));
	}

	Font::~Font()
	{

	}

	void Font::Draw(const glm::mat4& projection)
	{
		//tempShader->Bind();
		//tempShader->SetMat4("u_P", projection);
		//tempShader->SetMat4("u_M", glm::translate(glm::mat4(1.0f), glm::vec3(960.0f, 540.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(1024.0f, 1024.0f, 1.0f)));
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, m_AtlasTextureID);
		//glBindVertexArray(m_VAO);
		//glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		//glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	const std::string& Font::GetPath()
	{
		return m_Path;
	}

	unsigned int Font::GetSize()
	{
		return m_Size;
	}

	std::map<char, Character>& Font::GetCharacters()
	{
		return m_Characters;
	}

	GLuint Font::GetAtlasTextureID()
	{
		return m_AtlasTextureID;
	}

	void Font::SetHasFakeUser(bool hasFakeUser)
	{
		m_HasFakeUser = hasFakeUser;
	}

	bool Font::GetHasFakeUser()
	{
		return m_HasFakeUser;
	}
}