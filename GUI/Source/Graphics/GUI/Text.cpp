#include "Text.h"
#include "FontManager.h"
#include "Console.h"

// ADD KERNING

#include <glm/gtc/matrix_transform.hpp>

namespace GUI {
	Text::Text(const std::string& fontPath, unsigned int fontSize, const std::string& text, unsigned int numRows, unsigned int maxWidth)
		: m_Font(FontManager::GetInstance()->GetFont(fontPath, fontSize)), m_Text(text), m_NumberOfRows(numRows), m_MaxWidth(maxWidth), m_Scale(1.0f, 1.0f), m_Translation(0.0f, 0.0f)
	{
		// VAO VBO SIZE
		int stringLength = text.length();
        int vertexArraySize = stringLength * 24;

        GLfloat* vertices = new GLfloat[vertexArraySize];
        
        std::map<char, Character>& characters = m_Font->GetCharacters();

        float xOffset = 0.0f;

        for (int i = 0; i < stringLength; i++) {
            char c = m_Text[i];
            Character ch = characters[c];

            memcpy(vertices + 24*i, ch.vertices, 24 * sizeof(GLfloat));

            float shift = ch.bearing.x + xOffset;
            for (int j = 0; j < 24; j+=4) {
                vertices[j + 24 * i] += shift;
            }
            xOffset += (ch.advance >> 6);
            Console::Warning("Character: %c", c);
            Console::Info("     Width: %i", ch.size.x);
            Console::Info("     Advance: %li", (ch.advance >> 6));
            Console::Info("     Bearing: %i", ch.bearing.x);
        }

        glGenVertexArrays(1, &m_VAO);
        glBindVertexArray(m_VAO);
        
        glGenBuffers(1, &m_VBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        //glBufferData(GL_ARRAY_BUFFER, sizeof(combinedVertices), combinedVertices, GL_STATIC_DRAW);
        glBufferData(GL_ARRAY_BUFFER, vertexArraySize * sizeof(GLfloat), vertices, GL_DYNAMIC_DRAW);
        
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(sizeof(GLfloat) * 2));
        
        glFinish();

        //// update VBO for each character
        //float vertices[6][4] = {
        //    { xpos,     ypos + h,   0.0f, 0.0f },
        //    { xpos,     ypos,       0.0f, 1.0f },
        //    { xpos + w, ypos,       1.0f, 1.0f },
        //
        //    { xpos,     ypos + h,   0.0f, 0.0f },
        //    { xpos + w, ypos,       1.0f, 1.0f },
        //    { xpos + w, ypos + h,   1.0f, 0.0f }
        //};
        //// render glyph texture over quad
        //glBindTexture(GL_TEXTURE_2D, ch.textureID);
        //// update content of VBO memory
        //glBindBuffer(GL_ARRAY_BUFFER, VBO);
        //glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        //glBindBuffer(GL_ARRAY_BUFFER, 0);
        //// render quad
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        //// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        //x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
        //
        //glm::ivec2 finalSize = glm::ivec2(0, m_CharacterTextureCoordinates['A'].size.y);
        //finalSize.x += m_CharacterTextureCoordinates['A'].size.x;
        //finalSize.x += m_CharacterTextureCoordinates['S'].size.x;
        //finalSize.x += m_CharacterTextureCoordinates['S'].size.x;
        //GLfloat* combinedVertices = new GLfloat[72];
        //combinedVertices[0] = m_CharacterTextureCoordinates['A'].vertices[0] - (finalSize.x * 0.5);
        //combinedVertices[1] = m_CharacterTextureCoordinates['A'].vertices[1];
        //combinedVertices[2] = m_CharacterTextureCoordinates['A'].vertices[2];
        //combinedVertices[3] = m_CharacterTextureCoordinates['A'].vertices[3];
        //combinedVertices[4] = m_CharacterTextureCoordinates['A'].vertices[4] - (finalSize.x * 0.5);
        //combinedVertices[5] = m_CharacterTextureCoordinates['A'].vertices[5];
        //combinedVertices[6] = m_CharacterTextureCoordinates['A'].vertices[6];
        //combinedVertices[7] = m_CharacterTextureCoordinates['A'].vertices[7];
        //combinedVertices[8] = m_CharacterTextureCoordinates['A'].vertices[8] - (finalSize.x * 0.5);
        //combinedVertices[9] = m_CharacterTextureCoordinates['A'].vertices[9];
        //combinedVertices[10] = m_CharacterTextureCoordinates['A'].vertices[10];
        //combinedVertices[11] = m_CharacterTextureCoordinates['A'].vertices[11];
        //combinedVertices[12] = m_CharacterTextureCoordinates['A'].vertices[12] - (finalSize.x * 0.5);
        //combinedVertices[13] = m_CharacterTextureCoordinates['A'].vertices[13];
        //combinedVertices[14] = m_CharacterTextureCoordinates['A'].vertices[14];
        //combinedVertices[15] = m_CharacterTextureCoordinates['A'].vertices[15];
        //combinedVertices[16] = m_CharacterTextureCoordinates['A'].vertices[16] - (finalSize.x * 0.5);
        //combinedVertices[17] = m_CharacterTextureCoordinates['A'].vertices[17];
        //combinedVertices[18] = m_CharacterTextureCoordinates['A'].vertices[18];
        //combinedVertices[19] = m_CharacterTextureCoordinates['A'].vertices[19];
        //combinedVertices[20] = m_CharacterTextureCoordinates['A'].vertices[20] - (finalSize.x * 0.5);
        //combinedVertices[21] = m_CharacterTextureCoordinates['A'].vertices[21];
        //combinedVertices[22] = m_CharacterTextureCoordinates['A'].vertices[22];
        //combinedVertices[23] = m_CharacterTextureCoordinates['A'].vertices[23];
        //
        //combinedVertices[24] = m_CharacterTextureCoordinates['S'].vertices[0];
        //combinedVertices[25] = m_CharacterTextureCoordinates['S'].vertices[1];
        //combinedVertices[26] = m_CharacterTextureCoordinates['S'].vertices[2];
        //combinedVertices[27] = m_CharacterTextureCoordinates['S'].vertices[3];
        //combinedVertices[28] = m_CharacterTextureCoordinates['S'].vertices[4];
        //combinedVertices[29] = m_CharacterTextureCoordinates['S'].vertices[5];
        //combinedVertices[30] = m_CharacterTextureCoordinates['S'].vertices[6];
        //combinedVertices[31] = m_CharacterTextureCoordinates['S'].vertices[7];
        //combinedVertices[32] = m_CharacterTextureCoordinates['S'].vertices[8];
        //combinedVertices[33] = m_CharacterTextureCoordinates['S'].vertices[9];
        //combinedVertices[34] = m_CharacterTextureCoordinates['S'].vertices[10];
        //combinedVertices[35] = m_CharacterTextureCoordinates['S'].vertices[11];
        //combinedVertices[36] = m_CharacterTextureCoordinates['S'].vertices[12];
        //combinedVertices[37] = m_CharacterTextureCoordinates['S'].vertices[13];
        //combinedVertices[38] = m_CharacterTextureCoordinates['S'].vertices[14];
        //combinedVertices[39] = m_CharacterTextureCoordinates['S'].vertices[15];
        //combinedVertices[40] = m_CharacterTextureCoordinates['S'].vertices[16];
        //combinedVertices[41] = m_CharacterTextureCoordinates['S'].vertices[17];
        //combinedVertices[42] = m_CharacterTextureCoordinates['S'].vertices[18];
        //combinedVertices[43] = m_CharacterTextureCoordinates['S'].vertices[19];
        //combinedVertices[44] = m_CharacterTextureCoordinates['S'].vertices[20];
        //combinedVertices[45] = m_CharacterTextureCoordinates['S'].vertices[21];
        //combinedVertices[46] = m_CharacterTextureCoordinates['S'].vertices[22];
        //combinedVertices[47] = m_CharacterTextureCoordinates['S'].vertices[23];
        //
        //combinedVertices[48] = m_CharacterTextureCoordinates['S'].vertices[0] + (finalSize.x * 0.5);
        //combinedVertices[49] = m_CharacterTextureCoordinates['S'].vertices[1];
        //combinedVertices[50] = m_CharacterTextureCoordinates['S'].vertices[2];
        //combinedVertices[51] = m_CharacterTextureCoordinates['S'].vertices[3];
        //combinedVertices[52] = m_CharacterTextureCoordinates['S'].vertices[4] + (finalSize.x * 0.5);
        //combinedVertices[53] = m_CharacterTextureCoordinates['S'].vertices[5];
        //combinedVertices[54] = m_CharacterTextureCoordinates['S'].vertices[6];
        //combinedVertices[55] = m_CharacterTextureCoordinates['S'].vertices[7];
        //combinedVertices[56] = m_CharacterTextureCoordinates['S'].vertices[8] + (finalSize.x * 0.5);
        //combinedVertices[57] = m_CharacterTextureCoordinates['S'].vertices[9];
        //combinedVertices[58] = m_CharacterTextureCoordinates['S'].vertices[10];
        //combinedVertices[59] = m_CharacterTextureCoordinates['S'].vertices[11];
        //combinedVertices[60] = m_CharacterTextureCoordinates['S'].vertices[12] + (finalSize.x * 0.5);
        //combinedVertices[61] = m_CharacterTextureCoordinates['S'].vertices[13];
        //combinedVertices[62] = m_CharacterTextureCoordinates['S'].vertices[14];
        //combinedVertices[63] = m_CharacterTextureCoordinates['S'].vertices[15];
        //combinedVertices[64] = m_CharacterTextureCoordinates['S'].vertices[16] + (finalSize.x * 0.5);
        //combinedVertices[65] = m_CharacterTextureCoordinates['S'].vertices[17];
        //combinedVertices[66] = m_CharacterTextureCoordinates['S'].vertices[18];
        //combinedVertices[67] = m_CharacterTextureCoordinates['S'].vertices[19];
        //combinedVertices[68] = m_CharacterTextureCoordinates['S'].vertices[20] + (finalSize.x * 0.5);
        //combinedVertices[69] = m_CharacterTextureCoordinates['S'].vertices[21];
        //combinedVertices[70] = m_CharacterTextureCoordinates['S'].vertices[22];
        //combinedVertices[71] = m_CharacterTextureCoordinates['S'].vertices[23];
        //
        //glGenVertexArrays(1, &m_VAO);
        //glBindVertexArray(m_VAO);
        //
        //glGenBuffers(1, &m_VBO);
        //glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        ////glBufferData(GL_ARRAY_BUFFER, sizeof(combinedVertices), combinedVertices, GL_STATIC_DRAW);
        //glBufferData(GL_ARRAY_BUFFER, vertexArraySize * sizeof(GLfloat), combinedVertices, GL_DYNAMIC_DRAW);
        //
        //glEnableVertexAttribArray(0);
        //glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
        //glEnableVertexAttribArray(1);
        //glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(sizeof(GLfloat) * 2));
        //
		//glFinish();
	}

	Text::~Text()
	{
		glDeleteBuffers(1, &m_VBO);
		glDeleteVertexArrays(1, &m_VAO);
	}

	void Text::SetText(const std::string& newText, unsigned int fontSize)
	{
        m_Text = newText;
		glFinish();
	}

	void Text::SetNumberOfRows(unsigned int numRows)
	{
        m_NumberOfRows = numRows;
		glFinish();
	}

    void Text::SetMaxWidth(unsigned int maxWidth)
    {
        m_MaxWidth = maxWidth;
        glFinish();
    }

	void Text::SetTranslation(const glm::vec2& translation)
	{
		m_Translation = translation;
	}

	void Text::SetScale(const glm::vec2& scale)
	{
		m_Scale = scale;
	}

	const std::string& Text::GetText()
	{
		return m_Text;
	}

	unsigned int Text::GetNumberOfRows()
	{
		return m_NumberOfRows;
	}

    unsigned int Text::GetMaxWidth()
    {
        return m_MaxWidth;
    }

	const glm::vec2& Text::GetTranslation()
	{
		return m_Translation;
	}

	const glm::vec2& Text::GetScale()
	{
		return m_Scale;
	}

	void Text::Draw(const glm::mat4& projection, Shader& shader)
	{
        shader.Bind();
        shader.SetMat4("u_P", projection);
        //tempShader->SetMat4("u_M", glm::translate(glm::mat4(1.0f), glm::vec3(960.0f, 540.0f, 0.0f)));
        shader.SetMat4("u_M", glm::translate(glm::mat4(1.0f), glm::vec3(960.0f, 540.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(m_Scale, 1.0f)));
        //tempShader->SetMat4("u_M", glm::translate(glm::mat4(1.0f), glm::vec3(960.0f, 540.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(2048.0f, 2048.0f, 1.0f)));
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_Font->GetAtlasTextureID());
        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glDrawArrays(GL_TRIANGLES, 0, 6 * m_Text.length());
	}
}