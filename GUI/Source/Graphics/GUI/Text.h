#pragma once

#include "Font.h"

#include <vector>
#include <memory>
#include <glew.h>
#include <glm.hpp>
#include <string>

#define TEXT_ALIGN_LOCATION_LEFT 1
#define TEXT_ALIGN_LOCATION_CENTER 2
#define TEXT_ALIGN_LOCATION_RIGHT 4

namespace GUI {
	class Text {
	private:
		bool m_SupportsMarkup;

		GLuint m_VAO;
		GLuint m_VBO;
		glm::vec2 m_TranslationOffset;

		glm::vec2 m_Max;
		glm::vec2 m_Min;

		std::vector<std::shared_ptr<Font>> m_Fonts;
		std::string m_Text;

		unsigned int m_NumberOfRows;
		unsigned int m_MaxWidth;
		unsigned int m_TextAlignment;

		glm::vec4 m_Color;
	protected:
		glm::vec2 m_Scale;
		glm::vec2 m_Translation;
	public:
		Text(const std::string& fontPath, unsigned int fontSize, bool supportsMarkup, unsigned int numRows, unsigned int maxWidth, unsigned int alignment, const char* fmt, ...);
		~Text();

		void SetText(const std::string& text, const std::string& fontPath = std::string("keepFont"), int fontSize = -1, bool supportsMarkup = false, int numRows = -1, int alignment = -1);
		void SetText(const char* fmt, ...);
		void SetNumberOfRows(unsigned int numRows);
		void SetMaxWidth(unsigned int maxWidth);
		void SetTranslation(const glm::vec2& translation);
		void SetScale(const glm::vec2& scale);
		void SetColor(const glm::vec4& color);

		const std::string& GetText();
		unsigned int GetNumberOfRows();
		unsigned int GetMaxWidth();
		const glm::vec2& GetTranslation();
		const glm::vec2& GetScale();
		const glm::vec4& GetColor();

		void Draw(const glm::mat4& projection, Shader& shader);
	};
}