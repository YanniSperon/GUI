#include "Font.h"
#include "Console.h"
#include "FreetypeManager.h"

#include <glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

// ADD SUPPORT FOR ADVANCE Y

#define ATLAS_WIDTH 1024
#define ATLAS_HEIGHT 1024
#define INVERSE_ATLAS_WIDTH (1.0f / (float) ATLAS_WIDTH)
#define INVERSE_ATLAS_HEIGHT (1.0f / (float) ATLAS_HEIGHT)

namespace GUI {
	Font::Font(const std::string& path, unsigned int size)
		: m_Path(path), m_Characters(), m_Size(size), tempShader(new Shader("Resources/Shaders/Text", SHADER_FRAGMENT_SHADER | SHADER_VERTEX_SHADER))
	{
		glGenTextures(1, &m_AtlasTextureID);

		glBindTexture(GL_TEXTURE_2D, m_AtlasTextureID);
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

			for (unsigned char c = 0; c < 255; c++)
			{
				if (FT_Load_Char(face, c, FT_LOAD_RENDER))
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

				glm::ivec2 charSize = glm::ivec2(width, height);
				glm::vec2 charSizeFloat = glm::vec2(charSize);
				glm::ivec2 bearing = glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top);
				glm::vec2 bearingFloat = glm::vec2(bearing.x, bearing.y);

				float floatHalfWidth = floatWidth * 0.5f;
				float floatHalfHeight = floatHeight * 0.5f;

				glm::vec2 bottomLeftTexCoord = glm::vec2(floatXOffset * INVERSE_ATLAS_WIDTH, (floatYOffset + floatHeight) * INVERSE_ATLAS_HEIGHT);
				glm::vec2 topRightTexCoord = glm::vec2((floatXOffset + floatWidth) * INVERSE_ATLAS_WIDTH, floatYOffset * INVERSE_ATLAS_HEIGHT);

				if (c == 'T' || c == 'e' || c == 'S' || c == 't') {
					Console::Info("\'%c\' - Char Size - %.3f, %.3f", c, charSizeFloat.x, charSizeFloat.y);
					Console::Info("\'%c\' - Bearing - %.3f, %.3f", c, bearingFloat.x, bearingFloat.y);
					Console::Info("\'%c\' - Est Val - %.3f, %.3f", c, -floatHalfWidth, -floatHalfHeight);
					Console::Info("\'%c\' - Est Val2 - %.3f, %.3f", c, -floatHalfWidth + bearingFloat.x, -floatHalfHeight - (charSizeFloat.y - bearingFloat.y));
				}

				m_Characters[c] = {
					bottomLeftTexCoord,
					topRightTexCoord,
					charSize,
					bearing,
					face->glyph->advance.x,
					{
						0.0f, bearingFloat.y - charSizeFloat.y,              // Bottom Left
						bottomLeftTexCoord.x , bottomLeftTexCoord.y,    // 
						0.0f, bearingFloat.y,               // Top Left
						bottomLeftTexCoord.x, topRightTexCoord.y,       // 
						charSizeFloat.x, bearingFloat.y,                // Top Right
						topRightTexCoord.x, topRightTexCoord.y,         // 

						0.0f, bearingFloat.y - charSizeFloat.y,              // Bottom Left
						bottomLeftTexCoord.x, bottomLeftTexCoord.y,     // 
						charSizeFloat.x, bearingFloat.y,                // Top Right
						topRightTexCoord.x, topRightTexCoord.y,         // 
						charSizeFloat.x, bearingFloat.y - charSizeFloat.y,               // Bottom Right
						topRightTexCoord.x, bottomLeftTexCoord.y        // 
					}
				};
				
				//m_Characters[c] = {
				//	bottomLeftTexCoord,
				//	topRightTexCoord,
				//	charSize,
				//	bearing,
				//	(unsigned int)face->glyph->advance.x,
				//	{
				//		-floatHalfWidth + bearingFloat.x, -floatHalfHeight + bearingFloat.y,              // Bottom Left
				//		bottomLeftTexCoord.x , bottomLeftTexCoord.y,    // 
				//		-floatHalfWidth + bearingFloat.x, floatHalfHeight + bearingFloat.y,               // Top Left
				//		bottomLeftTexCoord.x, topRightTexCoord.y,       // 
				//		floatHalfWidth + bearingFloat.x, floatHalfHeight + bearingFloat.y,                // Top Right
				//		topRightTexCoord.x, topRightTexCoord.y,         // 
				//
				//		-floatHalfWidth + bearingFloat.x, -floatHalfHeight + bearingFloat.y,              // Bottom Left
				//		bottomLeftTexCoord.x, bottomLeftTexCoord.y,     // 
				//		floatHalfWidth + bearingFloat.x, floatHalfHeight + bearingFloat.y,                // Top Right
				//		topRightTexCoord.x, topRightTexCoord.y,         // 
				//		floatHalfWidth + bearingFloat.x, -floatHalfHeight + bearingFloat.y,               // Bottom Right
				//		topRightTexCoord.x, bottomLeftTexCoord.y        // 
				//	}
				//};

				//m_Characters[c] = {
				//	bottomLeftTexCoord,
				//	topRightTexCoord,
				//	charSize,
				//	bearing,
				//	(unsigned int)face->glyph->advance.x,
				//	{
				//		-floatHalfWidth, -floatHalfHeight,              // Bottom Left
				//		bottomLeftTexCoord.x , bottomLeftTexCoord.y,    // 
				//		-floatHalfWidth, floatHalfHeight,               // Top Left
				//		bottomLeftTexCoord.x, topRightTexCoord.y,       // 
				//		floatHalfWidth, floatHalfHeight,                // Top Right
				//		topRightTexCoord.x, topRightTexCoord.y,         // 
				//
				//		-floatHalfWidth, -floatHalfHeight,              // Bottom Left
				//		bottomLeftTexCoord.x, bottomLeftTexCoord.y,     // 
				//		floatHalfWidth, floatHalfHeight,                // Top Right
				//		topRightTexCoord.x, topRightTexCoord.y,         // 
				//		floatHalfWidth, -floatHalfHeight,               // Bottom Right
				//		topRightTexCoord.x, bottomLeftTexCoord.y        // 
				//	}
				//};
				
				//m_Characters[c] = {
				//	bottomLeftTexCoord,
				//	topRightTexCoord,
				//	charSize,
				//	bearing,
				//	(unsigned int)face->glyph->advance.x,
				//	{
				//		-floatHalfWidth + bearingFloat.x, -floatHalfHeight - bearingFloat.y,              // Bottom Left
				//		bottomLeftTexCoord.x , bottomLeftTexCoord.y,    // 
				//		-floatHalfWidth + bearingFloat.x, floatHalfHeight - bearingFloat.y,               // Top Left
				//		bottomLeftTexCoord.x, topRightTexCoord.y,       // 
				//		floatHalfWidth + bearingFloat.x, floatHalfHeight - bearingFloat.y,                // Top Right
				//		topRightTexCoord.x, topRightTexCoord.y,         // 
				//
				//		-floatHalfWidth + bearingFloat.x, -floatHalfHeight - bearingFloat.y,              // Bottom Left
				//		bottomLeftTexCoord.x, bottomLeftTexCoord.y,     // 
				//		floatHalfWidth + bearingFloat.x, floatHalfHeight - bearingFloat.y,                // Top Right
				//		topRightTexCoord.x, topRightTexCoord.y,         // 
				//		floatHalfWidth + bearingFloat.x, -floatHalfHeight - bearingFloat.y,               // Bottom Right
				//		topRightTexCoord.x, bottomLeftTexCoord.y        // 
				//	}
				//};

				//m_Characters[c] = {
				//	bottomLeftTexCoord,
				//	topRightTexCoord,
				//	charSize,
				//	bearing,
				//	(unsigned int)face->glyph->advance.x,
				//	{
				//		-floatHalfWidth + bearingFloat.x, -floatHalfHeight - (charSizeFloat.y - bearingFloat.y),              // Bottom Left
				//		bottomLeftTexCoord.x , bottomLeftTexCoord.y,    // 
				//		-floatHalfWidth + bearingFloat.x, floatHalfHeight - (charSizeFloat.y - bearingFloat.y),               // Top Left
				//		bottomLeftTexCoord.x, topRightTexCoord.y,       // 
				//		floatHalfWidth + bearingFloat.x, floatHalfHeight - (charSizeFloat.y - bearingFloat.y),                // Top Right
				//		topRightTexCoord.x, topRightTexCoord.y,         // 
				//
				//		-floatHalfWidth + bearingFloat.x, -floatHalfHeight - (charSizeFloat.y - bearingFloat.y),              // Bottom Left
				//		bottomLeftTexCoord.x, bottomLeftTexCoord.y,     // 
				//		floatHalfWidth + bearingFloat.x, floatHalfHeight - (charSizeFloat.y - bearingFloat.y),                // Top Right
				//		topRightTexCoord.x, topRightTexCoord.y,         // 
				//		floatHalfWidth + bearingFloat.x, -floatHalfHeight - (charSizeFloat.y - bearingFloat.y),               // Bottom Right
				//		topRightTexCoord.x, bottomLeftTexCoord.y        // 
				//	}
				//};

				//m_Characters[c] = {
				//	bottomLeftTexCoord,
				//	topRightTexCoord,
				//	charSize,
				//	bearing,
				//	(unsigned int)face->glyph->advance.x,
				//	{
				//		-floatHalfWidth + bearing.x, -floatHalfHeight,              // Bottom Left
				//		bottomLeftTexCoord.x , bottomLeftTexCoord.y,    // 
				//		-floatHalfWidth + bearing.x, floatHalfHeight,               // Top Left
				//		bottomLeftTexCoord.x, topRightTexCoord.y,       // 
				//		floatHalfWidth + bearing.x, floatHalfHeight,                // Top Right
				//		topRightTexCoord.x, topRightTexCoord.y,         // 
				//
				//		-floatHalfWidth + bearing.x, -floatHalfHeight,              // Bottom Left
				//		bottomLeftTexCoord.x, bottomLeftTexCoord.y,     // 
				//		floatHalfWidth + bearing.x, floatHalfHeight,                // Top Right
				//		topRightTexCoord.x, topRightTexCoord.y,         // 
				//		floatHalfWidth + bearing.x, -floatHalfHeight,               // Bottom Right
				//		topRightTexCoord.x, bottomLeftTexCoord.y        // 
				//	}
				//};
				if (c == '|') {
					Console::Info("Dimensions: (%d, %d)", m_Characters[c].size.x, m_Characters[c].size.y);
				}
				glTexSubImage2D(GL_TEXTURE_2D, 0, xOffset, yOffset, width, height, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

				xOffset += width + 4;
				if (xOffset + size >= ATLAS_WIDTH) {
					xOffset = 0;
					yOffset += size + 4;
					if (yOffset >= ATLAS_HEIGHT) {
						break;
					}
				}
			}
			FT_Done_Face(face);
		}

		//// draw capital A
		//GLfloat object[] = {
		//    -25.0f, -26.0f, // Bottom Left
		//    m_Characters['A'].bottomLeftTexCoord.x , m_Characters['A'].bottomLeftTexCoord.y,   // 
		//    -25.0f, 26.0f,  // Top Left
		//    m_Characters['A'].bottomLeftTexCoord.x, m_Characters['A'].topRightTexCoord.y,   // 
		//    25.0f, 26.0f,   // Top Right
		//    m_Characters['A'].topRightTexCoord.x, m_Characters['A'].topRightTexCoord.y,   // 
		//
		//    -25.0f, -26.0f, // Bottom Left
		//    m_Characters['A'].bottomLeftTexCoord.x, m_Characters['A'].bottomLeftTexCoord.y,   // 
		//    25.0f, 26.0f,   // Top Right
		//    m_Characters['A'].topRightTexCoord.x, m_Characters['A'].topRightTexCoord.y,   // 
		//    25.0f, -26.0f,  // Bottom Right
		//    m_Characters['A'].topRightTexCoord.x, m_Characters['A'].bottomLeftTexCoord.y    // 
		//};
		//
		//glGenVertexArrays(1, &tempVAO);
		//glBindVertexArray(tempVAO);
		//
		//glGenBuffers(1, &tempVBO);
		//glBindBuffer(GL_ARRAY_BUFFER, tempVBO);
		//glBufferData(GL_ARRAY_BUFFER, sizeof(object), object, GL_STATIC_DRAW);
		//
		//glEnableVertexAttribArray(0);
		//glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
		//glEnableVertexAttribArray(1);
		//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(sizeof(GLfloat) * 2));

		//glm::ivec2 finalSize = glm::ivec2(0, m_Characters['A'].size.y);
		//finalSize.x += m_Characters['A'].size.x;
		//finalSize.x += m_Characters['S'].size.x;
		//finalSize.x += m_Characters['S'].size.x;
		//GLfloat* combinedVertices = new GLfloat[72];
		//combinedVertices[0] = m_Characters['A'].vertices[0] - (finalSize.x * 0.5);
		//combinedVertices[1] = m_Characters['A'].vertices[1];
		//combinedVertices[2] = m_Characters['A'].vertices[2];
		//combinedVertices[3] = m_Characters['A'].vertices[3];
		//combinedVertices[4] = m_Characters['A'].vertices[4] - (finalSize.x * 0.5);
		//combinedVertices[5] = m_Characters['A'].vertices[5];
		//combinedVertices[6] = m_Characters['A'].vertices[6];
		//combinedVertices[7] = m_Characters['A'].vertices[7];
		//combinedVertices[8] = m_Characters['A'].vertices[8] - (finalSize.x * 0.5);
		//combinedVertices[9] = m_Characters['A'].vertices[9];
		//combinedVertices[10] = m_Characters['A'].vertices[10];
		//combinedVertices[11] = m_Characters['A'].vertices[11];
		//combinedVertices[12] = m_Characters['A'].vertices[12] - (finalSize.x * 0.5);
		//combinedVertices[13] = m_Characters['A'].vertices[13];
		//combinedVertices[14] = m_Characters['A'].vertices[14];
		//combinedVertices[15] = m_Characters['A'].vertices[15];
		//combinedVertices[16] = m_Characters['A'].vertices[16] - (finalSize.x * 0.5);
		//combinedVertices[17] = m_Characters['A'].vertices[17];
		//combinedVertices[18] = m_Characters['A'].vertices[18];
		//combinedVertices[19] = m_Characters['A'].vertices[19];
		//combinedVertices[20] = m_Characters['A'].vertices[20] - (finalSize.x * 0.5);
		//combinedVertices[21] = m_Characters['A'].vertices[21];
		//combinedVertices[22] = m_Characters['A'].vertices[22];
		//combinedVertices[23] = m_Characters['A'].vertices[23];
		//
		//combinedVertices[24] = m_Characters['S'].vertices[0];
		//combinedVertices[25] = m_Characters['S'].vertices[1];
		//combinedVertices[26] = m_Characters['S'].vertices[2];
		//combinedVertices[27] = m_Characters['S'].vertices[3];
		//combinedVertices[28] = m_Characters['S'].vertices[4];
		//combinedVertices[29] = m_Characters['S'].vertices[5];
		//combinedVertices[30] = m_Characters['S'].vertices[6];
		//combinedVertices[31] = m_Characters['S'].vertices[7];
		//combinedVertices[32] = m_Characters['S'].vertices[8];
		//combinedVertices[33] = m_Characters['S'].vertices[9];
		//combinedVertices[34] = m_Characters['S'].vertices[10];
		//combinedVertices[35] = m_Characters['S'].vertices[11];
		//combinedVertices[36] = m_Characters['S'].vertices[12];
		//combinedVertices[37] = m_Characters['S'].vertices[13];
		//combinedVertices[38] = m_Characters['S'].vertices[14];
		//combinedVertices[39] = m_Characters['S'].vertices[15];
		//combinedVertices[40] = m_Characters['S'].vertices[16];
		//combinedVertices[41] = m_Characters['S'].vertices[17];
		//combinedVertices[42] = m_Characters['S'].vertices[18];
		//combinedVertices[43] = m_Characters['S'].vertices[19];
		//combinedVertices[44] = m_Characters['S'].vertices[20];
		//combinedVertices[45] = m_Characters['S'].vertices[21];
		//combinedVertices[46] = m_Characters['S'].vertices[22];
		//combinedVertices[47] = m_Characters['S'].vertices[23];
		//
		//combinedVertices[48] = m_Characters['S'].vertices[0] + (finalSize.x * 0.5);
		//combinedVertices[49] = m_Characters['S'].vertices[1];
		//combinedVertices[50] = m_Characters['S'].vertices[2];
		//combinedVertices[51] = m_Characters['S'].vertices[3];
		//combinedVertices[52] = m_Characters['S'].vertices[4] + (finalSize.x * 0.5);
		//combinedVertices[53] = m_Characters['S'].vertices[5];
		//combinedVertices[54] = m_Characters['S'].vertices[6];
		//combinedVertices[55] = m_Characters['S'].vertices[7];
		//combinedVertices[56] = m_Characters['S'].vertices[8] + (finalSize.x * 0.5);
		//combinedVertices[57] = m_Characters['S'].vertices[9];
		//combinedVertices[58] = m_Characters['S'].vertices[10];
		//combinedVertices[59] = m_Characters['S'].vertices[11];
		//combinedVertices[60] = m_Characters['S'].vertices[12] + (finalSize.x * 0.5);
		//combinedVertices[61] = m_Characters['S'].vertices[13];
		//combinedVertices[62] = m_Characters['S'].vertices[14];
		//combinedVertices[63] = m_Characters['S'].vertices[15];
		//combinedVertices[64] = m_Characters['S'].vertices[16] + (finalSize.x * 0.5);
		//combinedVertices[65] = m_Characters['S'].vertices[17];
		//combinedVertices[66] = m_Characters['S'].vertices[18];
		//combinedVertices[67] = m_Characters['S'].vertices[19];
		//combinedVertices[68] = m_Characters['S'].vertices[20] + (finalSize.x * 0.5);
		//combinedVertices[69] = m_Characters['S'].vertices[21];
		//combinedVertices[70] = m_Characters['S'].vertices[22];
		//combinedVertices[71] = m_Characters['S'].vertices[23];

		GLfloat vertices[24] = {
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

		//GLfloat* combinedVertices = new GLfloat[72]{
		//    m_Characters['A'].vertices[0],
		//    m_Characters['A'].vertices[1],
		//    m_Characters['A'].vertices[2],
		//    m_Characters['A'].vertices[3],
		//    m_Characters['A'].vertices[4],
		//    m_Characters['A'].vertices[5],
		//    m_Characters['A'].vertices[6],
		//    m_Characters['A'].vertices[7],
		//    m_Characters['A'].vertices[8],
		//    m_Characters['A'].vertices[9],
		//    m_Characters['A'].vertices[10],
		//    m_Characters['A'].vertices[11],
		//    m_Characters['A'].vertices[12],
		//    m_Characters['A'].vertices[13],
		//    m_Characters['A'].vertices[14],
		//    m_Characters['A'].vertices[15],
		//    m_Characters['A'].vertices[16],
		//    m_Characters['A'].vertices[17],
		//    m_Characters['A'].vertices[18],
		//    m_Characters['A'].vertices[19],
		//    m_Characters['A'].vertices[20],
		//    m_Characters['A'].vertices[21],
		//    m_Characters['A'].vertices[22],
		//    m_Characters['A'].vertices[23],
		//
		//    m_Characters['S'].vertices[0],
		//    m_Characters['S'].vertices[1],
		//    m_Characters['S'].vertices[2],
		//    m_Characters['S'].vertices[3],
		//    m_Characters['S'].vertices[4],
		//    m_Characters['S'].vertices[5],
		//    m_Characters['S'].vertices[6],
		//    m_Characters['S'].vertices[7],
		//    m_Characters['S'].vertices[8],
		//    m_Characters['S'].vertices[9],
		//    m_Characters['S'].vertices[10],
		//    m_Characters['S'].vertices[11],
		//    m_Characters['S'].vertices[12],
		//    m_Characters['S'].vertices[13],
		//    m_Characters['S'].vertices[14],
		//    m_Characters['S'].vertices[15],
		//    m_Characters['S'].vertices[16],
		//    m_Characters['S'].vertices[17],
		//    m_Characters['S'].vertices[18],
		//    m_Characters['S'].vertices[19],
		//    m_Characters['S'].vertices[20],
		//    m_Characters['S'].vertices[21],
		//    m_Characters['S'].vertices[22],
		//    m_Characters['S'].vertices[23],
		//
		//    m_Characters['S'].vertices[0],
		//    m_Characters['S'].vertices[1],
		//    m_Characters['S'].vertices[2],
		//    m_Characters['S'].vertices[3],
		//    m_Characters['S'].vertices[4],
		//    m_Characters['S'].vertices[5],
		//    m_Characters['S'].vertices[6],
		//    m_Characters['S'].vertices[7],
		//    m_Characters['S'].vertices[8],
		//    m_Characters['S'].vertices[9],
		//    m_Characters['S'].vertices[10],
		//    m_Characters['S'].vertices[11],
		//    m_Characters['S'].vertices[12],
		//    m_Characters['S'].vertices[13],
		//    m_Characters['S'].vertices[14],
		//    m_Characters['S'].vertices[15],
		//    m_Characters['S'].vertices[16],
		//    m_Characters['S'].vertices[17],
		//    m_Characters['S'].vertices[18],
		//    m_Characters['S'].vertices[19],
		//    m_Characters['S'].vertices[20],
		//    m_Characters['S'].vertices[21],
		//    m_Characters['S'].vertices[22],
		//    m_Characters['S'].vertices[23]
		//};

		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);

		glGenBuffers(1, &m_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		//glBufferData(GL_ARRAY_BUFFER, sizeof(combinedVertices), combinedVertices, GL_STATIC_DRAW);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(sizeof(GLfloat) * 2));
	}

	Font::~Font()
	{

	}

	void Font::Draw(const glm::mat4& projection)
	{
		//tempShader->Bind();
		//tempShader->SetMat4("u_P", projection);
		//tempShader->SetMat4("u_M", glm::translate(glm::mat4(1.0f), glm::vec3(m_Translation, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(m_Scale, 1.0f)));
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, m_AtlasTextureID);
		//glBindVertexArray(m_VAO);
		//glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		//glDrawArrays(GL_TRIANGLES, 0, 6);

		//Console::Info("Size: (%d, %d)", m_Characters['S'].size.x, m_Characters['S'].size.y);

		//GLfloat* vertices = new GLfloat[72];
		//
		//glm::ivec2 finalSize = glm::ivec2(0, m_Characters['A'].size.y);
		//finalSize.x += m_Characters['A'].size.x;
		//finalSize.x += m_Characters['S'].size.x;
		//finalSize.x += m_Characters['S'].size.x;
		//Console::Info("before");
		//vertices[0 + 24 * 0] = m_Characters['A'].vertices[0];
		//vertices[1 + 24 * 0] = m_Characters['A'].vertices[1];
		//vertices[2 + 24 * 0] = m_Characters['A'].vertices[2];
		//vertices[3 + 24 * 0] = m_Characters['A'].vertices[3];
		//vertices[4 + 24 * 0] = m_Characters['A'].vertices[4];
		//vertices[5 + 24 * 0] = m_Characters['A'].vertices[5];
		//vertices[6 + 24 * 0] = m_Characters['A'].vertices[6];
		//vertices[7 + 24 * 0] = m_Characters['A'].vertices[7];
		//vertices[8 + 24 * 0] = m_Characters['A'].vertices[8];
		//vertices[9 + 24 * 0] = m_Characters['A'].vertices[9];
		//vertices[10 + 24 * 0] = m_Characters['A'].vertices[10];
		//vertices[11 + 24 * 0] = m_Characters['A'].vertices[11];
		//vertices[12 + 24 * 0] = m_Characters['A'].vertices[12];
		//vertices[13 + 24 * 0] = m_Characters['A'].vertices[13];
		//vertices[14 + 24 * 0] = m_Characters['A'].vertices[14];
		//vertices[15 + 24 * 0] = m_Characters['A'].vertices[15];
		//vertices[16 + 24 * 0] = m_Characters['A'].vertices[16];
		//vertices[17 + 24 * 0] = m_Characters['A'].vertices[17];
		//vertices[18 + 24 * 0] = m_Characters['A'].vertices[18];
		//vertices[19 + 24 * 0] = m_Characters['A'].vertices[19];
		//vertices[20 + 24 * 0] = m_Characters['A'].vertices[20];
		//vertices[21 + 24 * 0] = m_Characters['A'].vertices[21];
		//vertices[22 + 24 * 0] = m_Characters['A'].vertices[22];
		//vertices[23 + 24 * 0] = m_Characters['A'].vertices[23];
		//
		//vertices[0 + 24 * 1] = m_Characters['S'].vertices[0];
		//vertices[1 + 24 * 1] = m_Characters['S'].vertices[1];
		//vertices[2 + 24 * 1] = m_Characters['S'].vertices[2];
		//vertices[3 + 24 * 1] = m_Characters['S'].vertices[3];
		//vertices[4 + 24 * 1] = m_Characters['S'].vertices[4];
		//vertices[5 + 24 * 1] = m_Characters['S'].vertices[5];
		//vertices[6 + 24 * 1] = m_Characters['S'].vertices[6];
		//vertices[7 + 24 * 1] = m_Characters['S'].vertices[7];
		//vertices[8 + 24 * 1] = m_Characters['S'].vertices[8];
		//vertices[9 + 24 * 1] = m_Characters['S'].vertices[9];
		//vertices[10 + 24 * 1] = m_Characters['S'].vertices[10];
		//vertices[11 + 24 * 1] = m_Characters['S'].vertices[11];
		//vertices[12 + 24 * 1] = m_Characters['S'].vertices[12];
		//vertices[13 + 24 * 1] = m_Characters['S'].vertices[13];
		//vertices[14 + 24 * 1] = m_Characters['S'].vertices[14];
		//vertices[15 + 24 * 1] = m_Characters['S'].vertices[15];
		//vertices[16 + 24 * 1] = m_Characters['S'].vertices[16];
		//vertices[17 + 24 * 1] = m_Characters['S'].vertices[17];
		//vertices[18 + 24 * 1] = m_Characters['S'].vertices[18];
		//vertices[19 + 24 * 1] = m_Characters['S'].vertices[19];
		//vertices[20 + 24 * 1] = m_Characters['S'].vertices[20];
		//vertices[21 + 24 * 1] = m_Characters['S'].vertices[21];
		//vertices[22 + 24 * 1] = m_Characters['S'].vertices[22];
		//vertices[23 + 24 * 1] = m_Characters['S'].vertices[23];
		//
		//vertices[0 + 24 * 2] = m_Characters['S'].vertices[0];
		//vertices[1 + 24 * 2] = m_Characters['S'].vertices[1];
		//vertices[2 + 24 * 2] = m_Characters['S'].vertices[2];
		//vertices[3 + 24 * 2] = m_Characters['S'].vertices[3];
		//vertices[4 + 24 * 2] = m_Characters['S'].vertices[4];
		//vertices[5 + 24 * 2] = m_Characters['S'].vertices[5];
		//vertices[6 + 24 * 2] = m_Characters['S'].vertices[6];
		//vertices[7 + 24 * 2] = m_Characters['S'].vertices[7];
		//vertices[8 + 24 * 2] = m_Characters['S'].vertices[8];
		//vertices[9 + 24 * 2] = m_Characters['S'].vertices[9];
		//vertices[10 + 24 * 2] = m_Characters['S'].vertices[10];
		//vertices[11 + 24 * 2] = m_Characters['S'].vertices[11];
		//vertices[12 + 24 * 2] = m_Characters['S'].vertices[12];
		//vertices[13 + 24 * 2] = m_Characters['S'].vertices[13];
		//vertices[14 + 24 * 2] = m_Characters['S'].vertices[14];
		//vertices[15 + 24 * 2] = m_Characters['S'].vertices[15];
		//vertices[16 + 24 * 2] = m_Characters['S'].vertices[16];
		//vertices[17 + 24 * 2] = m_Characters['S'].vertices[17];
		//vertices[18 + 24 * 2] = m_Characters['S'].vertices[18];
		//vertices[19 + 24 * 2] = m_Characters['S'].vertices[19];
		//vertices[20 + 24 * 2] = m_Characters['S'].vertices[20];
		//vertices[21 + 24 * 2] = m_Characters['S'].vertices[21];
		//vertices[22 + 24 * 2] = m_Characters['S'].vertices[22];
		//vertices[23 + 24 * 2] = m_Characters['S'].vertices[23];
		//Console::Info("after");

		tempShader->Bind();
		tempShader->SetMat4("u_P", projection);
		//tempShader->SetMat4("u_M", glm::translate(glm::mat4(1.0f), glm::vec3(960.0f, 540.0f, 0.0f)));
		tempShader->SetMat4("u_M", glm::translate(glm::mat4(1.0f), glm::vec3(960.0f, 540.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(1024.0f, 1024.0f, 1.0f)));
		//tempShader->SetMat4("u_M", glm::translate(glm::mat4(1.0f), glm::vec3(960.0f, 540.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(2048.0f, 2048.0f, 1.0f)));
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_AtlasTextureID);
		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	std::map<char, Character>& Font::GetCharacters()
	{
		return m_Characters;
	}

	GLuint Font::GetAtlasTextureID()
	{
		return m_AtlasTextureID;
	}
}