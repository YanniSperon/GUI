#include "Text.h"
#include "FontManager.h"
#include "Console.h"
#include "MathExtensions.h"
#include "ShaderManager.h"

#include <cstdarg>
#include <glm/gtc/matrix_transform.hpp>

namespace GUI {
    Text::Text()
        : m_Fonts(), m_NumberOfRows(0), m_MaxWidth(1000), m_Scale(1.0f, 1.0f), m_Translation(0.0f, 0.0f), m_Min(FLT_MAX, FLT_MAX), m_Max(-FLT_MAX, -FLT_MAX), m_SupportsMarkup(false), m_Alignment(Alignment::LEFT), m_Color(1.0f, 1.0f, 1.0f, 1.0f), m_TranslationOffset(0.0f, 0.0f), m_Shader(ShaderManager::GetInstance()->GetShader("Resources/Shaders/Text", SHADER_VERTEX_SHADER | SHADER_FRAGMENT_SHADER)), m_FontPath(""), m_FontSize(12)
    {
        glGenVertexArrays(1, &m_VAO);
        glBindVertexArray(m_VAO);

        glGenBuffers(1, &m_VBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(sizeof(GLfloat) * 2));

        glFinish();
    }

    Text::Text(const std::string& fontPath, unsigned int fontSize, bool supportsMarkup, unsigned int numRows, unsigned int maxWidth, Alignment alignment, const char* fmt, ...)
        : m_Fonts(), m_NumberOfRows(numRows), m_MaxWidth(maxWidth), m_Scale(1.0f, 1.0f), m_Translation(0.0f, 0.0f), m_Min(FLT_MAX, FLT_MAX), m_Max(-FLT_MAX, -FLT_MAX), m_SupportsMarkup(supportsMarkup), m_Alignment(alignment), m_Color(1.0f, 1.0f, 1.0f, 1.0f), m_Shader(ShaderManager::GetInstance()->GetShader("Resources/Shaders/Text", SHADER_VERTEX_SHADER | SHADER_FRAGMENT_SHADER)), m_FontPath(fontPath), m_FontSize(fontSize)
    {
        if (m_Alignment == Alignment::PREVIOUS || m_Alignment == Alignment::DEFAULT) {
            m_Alignment = Alignment::LEFT;
        }
        va_list args, argsCopy;
        va_start(args, fmt);
        va_copy(argsCopy, args);
        size_t bufferSize = vsnprintf(nullptr, 0, fmt, argsCopy) + 1;
        char* message = new char[bufferSize];
        vsnprintf(message, bufferSize, fmt, args);
        m_Text = std::string(message);
        va_end(args);
        va_end(argsCopy);
        delete[] message;

        int stringLength = m_Text.length();
        int vertexArraySize = stringLength * 24;

        GLfloat* vertices = new GLfloat[vertexArraySize];


        if (m_SupportsMarkup) {

        } else {
            m_Fonts.push_back(FontManager::GetInstance()->GetFont(fontPath, fontSize));
        }

        Recalculate();
    }

    Text::~Text()
    {
        glDeleteBuffers(1, &m_VBO);
        glDeleteVertexArrays(1, &m_VAO);
        FontManager::GetInstance()->MarkForCleanup();
    }

    void Text::Recalculate()
    {
        int stringLength = m_Text.length();
        int vertexArraySize = stringLength * 24;

        GLfloat* vertices = new GLfloat[vertexArraySize];


        if (m_SupportsMarkup) {
            // Default Font
            //m_Fonts.push_back(FontManager::GetInstance()->GetFont(fontPath, fontSize));

            Console::Warning("Text does not currently support markup!");
        }
        else {
            m_Fonts.clear();
            m_Fonts.push_back(FontManager::GetInstance()->GetFont(m_FontPath, m_FontSize));
            FontManager::GetInstance()->MarkForCleanup();

            std::map<char, Character>& characters = m_Fonts[0]->GetCharacters();

            float xOffset = 0.0f;
            float yOffset = 0.0f;
            for (int i = 0; i < stringLength; i++) {
                char c = m_Text[i];
                if (c == 'k') {
                    Console::Info("K");
                }
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

                int characterBearingX = ch.bearing.x;

                float tempXOffset = kerningXPx + (ch.advance.x >> 6);
                if (xOffset + tempXOffset > m_MaxWidth) {
                    xOffset = 0.0f;
                    yOffset += m_Fonts[0]->GetSize();
                    kerningXPx = 0;
                    characterBearingX = 0;
                }

                float shiftX = kerningXPx + characterBearingX + xOffset;
                for (int j = 0; j < 24; j += 4) {
                    vertices[j + 24 * i] += shiftX;
                    vertices[j + 24 * i + 1] -= yOffset;
                    //Console::Info("Shift: (%f, %f)", vertices[j + 24 * i], vertices[j + 24 * i + 1]);
                }
                float minX = vertices[24 * i];
                float minY = vertices[1 + 24 * i];
                m_Min.x = glm::min(minX, m_Min.x);
                m_Min.y = glm::min(minY, m_Min.y);
                float maxX = vertices[8 + 24 * i];
                float maxY = vertices[9 + 24 * i];
                m_Max.x = glm::max(maxX, m_Max.x);
                m_Max.y = glm::max(maxY, m_Max.y);
                xOffset += tempXOffset;
            }
        }
        m_TranslationOffset = glm::round(m_Max - ((glm::abs(m_Min) + glm::abs(m_Max)) * 0.5f));

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

    void Text::SetText(const std::string& text, const std::string& fontPath, int fontSize, bool supportsMarkup, int numRows, Alignment alignment)
    {
        switch (alignment)
        {
        case GUI::Text::Alignment::DEFAULT:
            m_Alignment = Alignment::LEFT;
            break;
        case GUI::Text::Alignment::PREVIOUS:
            break;
        case GUI::Text::Alignment::LEFT:
            m_Alignment = Alignment::LEFT;
            break;
        case GUI::Text::Alignment::CENTER:
            m_Alignment = Alignment::CENTER;
            break;
        case GUI::Text::Alignment::RIGHT:
            m_Alignment = Alignment::RIGHT;
            break;
        default:
            break;
        }

        m_Text = text;
        
        m_Min = glm::vec2(FLT_MAX, FLT_MAX);
        m_Max = glm::vec2(-FLT_MAX, -FLT_MAX);

        if (numRows > 0) {
            m_NumberOfRows = numRows;
        }

        if (fontPath != "keepFont") {
            m_FontPath = fontPath;
        }

        if (fontSize > 0) {
            m_FontSize = fontSize;
        }

        m_SupportsMarkup = supportsMarkup;

        Recalculate();
    }

    void Text::SetText(const char* fmt, ...)
    {
        va_list args, argsCopy;
        va_start(args, fmt);
        va_copy(argsCopy, args);
        size_t bufferSize = vsnprintf(nullptr, 0, fmt, argsCopy) + 1;
        char* message = new char[bufferSize];
        vsnprintf(message, bufferSize, fmt, args);
        m_Text = std::string(message);
        va_end(args);
        va_end(argsCopy);
        delete[] message;

        Recalculate();
    }

    void Text::SetNumberOfRows(unsigned int numRows)
    {
        if (m_NumberOfRows != numRows) {
            m_NumberOfRows = numRows;
            Recalculate();
        }
    }

    void Text::SetMaxWidth(unsigned int maxWidth)
    {
        if (m_MaxWidth != maxWidth) {
            m_MaxWidth = maxWidth;
            Recalculate();
        }
    }

    void Text::SetTranslation(const glm::vec2& translation)
    {
        m_Translation = translation;
    }

    void Text::SetScale(const glm::vec2& scale)
    {
        m_Scale = scale;
    }

    void Text::SetColor(const glm::vec4& color)
    {
        m_Color = color;
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

    const glm::vec4& Text::GetColor()
    {
        return m_Color;
    }

    void Text::Draw(const glm::mat4& projection)
    {
        if (m_SupportsMarkup) {
            Console::Warning("Text does not currently support markup!");
        }
        else {
            m_Shader->Bind();
            m_Shader->SetMat4("u_P", projection);
            m_Shader->SetMat4("u_M", glm::translate(glm::mat4(1.0f), glm::vec3(m_Translation - m_TranslationOffset, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(m_Scale, 1.0f)));
            m_Shader->SetVec4("u_Color", m_Color);
            glActiveTexture(GL_TEXTURE0);
            GLuint textureID = 0;
            if (m_Fonts.size() > 0) {
                textureID = m_Fonts[0]->GetAtlasTextureID();
            }
            glBindTexture(GL_TEXTURE_2D, textureID);
            glBindVertexArray(m_VAO);
            glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
            glDrawArrays(GL_TRIANGLES, 0, 6 * m_Text.length());
        }
    }
}