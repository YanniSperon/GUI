#include "Text.h"
#include "FontManager.h"
#include "Console.h"

#include <cstdarg>
#include <glm/gtc/matrix_transform.hpp>

namespace GUI {
	Text::Text(const std::string& fontPath, unsigned int fontSize, unsigned int numRows, unsigned int maxWidth, const char* fmt, ...)
		: m_Font(FontManager::GetInstance()->GetFont(fontPath, fontSize)), m_NumberOfRows(numRows), m_MaxWidth(maxWidth), m_Scale(1.0f, 1.0f), m_Translation(0.0f, 0.0f), m_Min(FLT_MAX, FLT_MAX), m_Max(-FLT_MAX, -FLT_MAX)
	{
        va_list args;
        va_start(args, fmt);
        int bufferSize = 1024 + strlen(fmt) * 2;
        char* message = new char[bufferSize];
        vsnprintf(message, bufferSize, fmt, args);
        m_Text = std::string(message);
        va_end(args);
        delete[] message;

		int stringLength = m_Text.length();
        int vertexArraySize = stringLength * 24;

        GLfloat* vertices = new GLfloat[vertexArraySize];
        
        std::map<char, Character>& characters = m_Font->GetCharacters();

        float xOffset = 0.0f;

        for (int i = 0; i < stringLength; i++) {
            char c = m_Text[i];
            Character ch = characters[c];

            memcpy(vertices + 24*i, ch.vertices, 24 * sizeof(GLfloat));

            int kerningX = 0;
            if (i != 0) {
                Character previousChar = characters[m_Text[i-1]];
                if (previousChar.kerning.find(c) != previousChar.kerning.end()) {
                    kerningX = previousChar.kerning[c].x;
                }
            }

            int kerningXPx = (kerningX >> 6);

            float shiftX = kerningXPx + ch.bearing.x + xOffset;
            for (int j = 0; j < 24; j += 4) {
                vertices[j + 24 * i] += shiftX;
            }
            float minX = vertices[24 * i];
            float minY = vertices[1 + 24 * i];
            m_Min.x = glm::min(minX, m_Min.x);
            m_Min.y = glm::min(minY, m_Min.y);
            float maxX = vertices[8 + 24 * i];
            float maxY = vertices[9 + 24 * i];
            m_Max.x = glm::max(maxX, m_Max.x);
            m_Max.y = glm::max(maxY, m_Max.y);
            xOffset += kerningXPx + (ch.advance.x >> 6);
        }
        m_TranslationOffset = m_Max - ((glm::abs(m_Min) + glm::abs(m_Max)) * 0.5f);

        glGenVertexArrays(1, &m_VAO);
        glBindVertexArray(m_VAO);
        
        glGenBuffers(1, &m_VBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, vertexArraySize * sizeof(GLfloat), vertices, GL_DYNAMIC_DRAW);
        
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(sizeof(GLfloat) * 2));

        glFinish();

        delete[] vertices;
    }

	Text::~Text()
	{
		glDeleteBuffers(1, &m_VBO);
		glDeleteVertexArrays(1, &m_VAO);
	}

    void Text::SetText(const std::string& text, const std::string& fontPath, int fontSize, int numRows)
    {
        m_Text = text;

        m_Min = glm::vec2(FLT_MAX, FLT_MAX);
        m_Max = glm::vec2(-FLT_MAX, -FLT_MAX);

        std::string tempFontPath = fontPath;
        if (fontPath == "keepFont") {
            tempFontPath = m_Font->GetPath();
        }
        int tempFontSize = fontSize;
        if (fontSize < 0) {
            tempFontSize = m_Font->GetSize();
        }
        int tempNumRows = numRows;
        if (numRows < 0) {
            tempNumRows = m_NumberOfRows;
        }
        m_Font = FontManager::GetInstance()->GetFont(tempFontPath, tempFontSize);
        m_NumberOfRows = tempNumRows;

        int stringLength = m_Text.length();
        int vertexArraySize = stringLength * 24;

        GLfloat* vertices = new GLfloat[vertexArraySize];

        std::map<char, Character>& characters = m_Font->GetCharacters();

        float xOffset = 0.0f;

        for (int i = 0; i < stringLength; i++) {
            char c = m_Text[i];
            Character ch = characters[c];

            memcpy(vertices + 24 * i, ch.vertices, 24 * sizeof(GLfloat));

            int kerningX = 0;
            if (i != 0) {
                Character previousChar = characters[m_Text[i - 1]];
                if (previousChar.kerning.find(c) != previousChar.kerning.end()) {
                    kerningX = previousChar.kerning[c].x;
                }
            }

            int kerningXPx = (kerningX >> 6);

            float shiftX = kerningXPx + ch.bearing.x + xOffset;
            for (int j = 0; j < 24; j += 4) {
                vertices[j + 24 * i] += shiftX;
            }
            float minX = vertices[24 * i];
            float minY = vertices[1 + 24 * i];
            m_Min.x = glm::min(minX, m_Min.x);
            m_Min.y = glm::min(minY, m_Min.y);
            float maxX = vertices[8 + 24 * i];
            float maxY = vertices[9 + 24 * i];
            m_Max.x = glm::max(maxX, m_Max.x);
            m_Max.y = glm::max(maxY, m_Max.y);
            xOffset += kerningXPx + (ch.advance.x >> 6);
        }
        m_TranslationOffset = m_Max - ((glm::abs(m_Min) + glm::abs(m_Max)) * 0.5f);

        glBindVertexArray(m_VAO);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, vertexArraySize * sizeof(GLfloat), vertices, GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(sizeof(GLfloat) * 2));

        glFinish();

        delete[] vertices;
    }

    void Text::SetText(const char* fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        int bufferSize = 1024 + strlen(fmt) * 2;
        char* message = new char[bufferSize];
        vsnprintf(message, bufferSize, fmt, args);
        m_Text = std::string(message);
        va_end(args);
        delete[] message;

        m_Min = glm::vec2(FLT_MAX, FLT_MAX);
        m_Max = glm::vec2(-FLT_MAX, -FLT_MAX);

        int stringLength = m_Text.length();
        int vertexArraySize = stringLength * 24;

        GLfloat* vertices = new GLfloat[vertexArraySize];

        std::map<char, Character>& characters = m_Font->GetCharacters();

        float xOffset = 0.0f;

        for (int i = 0; i < stringLength; i++) {
            char c = m_Text[i];
            Character ch = characters[c];

            memcpy(vertices + 24 * i, ch.vertices, 24 * sizeof(GLfloat));

            int kerningX = 0;
            if (i != 0) {
                Character previousChar = characters[m_Text[i - 1]];
                if (previousChar.kerning.find(c) != previousChar.kerning.end()) {
                    kerningX = previousChar.kerning[c].x;
                }
            }

            int kerningXPx = (kerningX >> 6);

            float shiftX = kerningXPx + ch.bearing.x + xOffset;
            for (int j = 0; j < 24; j += 4) {
                vertices[j + 24 * i] += shiftX;
            }
            float minX = vertices[24 * i];
            float minY = vertices[1 + 24 * i];
            m_Min.x = glm::min(minX, m_Min.x);
            m_Min.y = glm::min(minY, m_Min.y);
            float maxX = vertices[8 + 24 * i];
            float maxY = vertices[9 + 24 * i];
            m_Max.x = glm::max(maxX, m_Max.x);
            m_Max.y = glm::max(maxY, m_Max.y);
            xOffset += kerningXPx + (ch.advance.x >> 6);
        }
        m_TranslationOffset = m_Max - ((glm::abs(m_Min) + glm::abs(m_Max)) * 0.5f);

        glBindVertexArray(m_VAO);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, vertexArraySize * sizeof(GLfloat), vertices, GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(sizeof(GLfloat) * 2));

        glFinish();

        delete[] vertices;
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
        shader.SetMat4("u_M", glm::translate(glm::mat4(1.0f), glm::vec3(960.0f, 540.0f, 0.0f) - glm::vec3(m_TranslationOffset, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(m_Scale, 1.0f)));
        //tempShader->SetMat4("u_M", glm::translate(glm::mat4(1.0f), glm::vec3(960.0f, 540.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(2048.0f, 2048.0f, 1.0f)));
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_Font->GetAtlasTextureID());
        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glDrawArrays(GL_TRIANGLES, 0, 6 * m_Text.length());
	}
}