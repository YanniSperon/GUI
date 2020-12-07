#include "Font.h"
#include "Console.h"
#include "FreetypeManager.h"

#include <glew.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#define ATLAS_WIDTH 1024
#define ATLAS_HEIGHT 1024

namespace GUI {
	Font::Font(const std::string& path, int size)
		: m_Path(path), m_CharacterTextureCoordinates(), m_Size(size), tempShader(new Shader("Resources/Shaders/Temp", SHADER_FRAGMENT_SHADER | SHADER_VERTEX_SHADER))
	{
        glGenTextures(1, &m_TextureAtlasID);

        glBindTexture(GL_TEXTURE_2D, m_TextureAtlasID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, ATLAS_WIDTH, ATLAS_HEIGHT, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

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

            for (unsigned char c = 0; c < 128; c++)
            {
                if (FT_Load_Char(face, c, FT_LOAD_RENDER))
                {
                    Console::Error("Failed to load Glyph");
                    continue;
                }

                int width = face->glyph->bitmap.width;
                int height = face->glyph->bitmap.rows;

                Character character = {
                    glm::vec2(xOffset, yOffset),
                    glm::vec2(xOffset + width, yOffset + height),
                    glm::ivec2(width, height),
                    glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                    face->glyph->advance.x
                };
                m_CharacterTextureCoordinates[c] = character;
                glTexSubImage2D(GL_TEXTURE_2D, 0, xOffset, yOffset, width, height, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

                xOffset += width + 1;
                if (xOffset + size >= ATLAS_WIDTH) {
                    xOffset = 0;
                    yOffset += size + 1;
                }
            }
            FT_Done_Face(face);
		}

        GLfloat vertices[] = {
            -0.5f, -0.5f, // Bottom Left
            0.0f, 1.0f,   // 
            -0.5f, 0.5f,  // Top Left
            0.0f, 0.0f,   // 
            0.5f, 0.5f,   // Top Right
            1.0f, 0.0f,   // 

            -0.5f, -0.5f, // Bottom Left
            0.0f, 1.0f,   // 
            0.5f, 0.5f,   // Top Right
            1.0f, 0.0f,   // 
            0.5f, -0.5f,  // Bottom Right
            1.0f, 1.0f    // 
        };

        glGenVertexArrays(1, &m_VAO);
        glBindVertexArray(m_VAO);

        glGenBuffers(1, &m_VBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(sizeof(GLfloat) * 2));
	}

	Font::~Font()
	{

	}

    void Font::Draw()
    {
        tempShader->Bind();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_TextureAtlasID);
        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
}