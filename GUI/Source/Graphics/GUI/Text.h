#pragma once

#include "Font.h"

#include <glew.h>
#include <glm.hpp>
#include <string>

namespace GUI {
	class Text {
	private:
		bool m_SupportsMarkup;

		GLuint m_VAO;
		GLuint m_VBO;
		glm::vec2 m_TranslationOffset;

		glm::vec2 m_Max;
		glm::vec2 m_Min;

		Font* m_Font;
		std::string m_Text;

		unsigned int m_NumberOfRows;
		unsigned int m_MaxWidth;
	protected:
		glm::vec2 m_Scale;
		glm::vec2 m_Translation;
	public:
		Text(const std::string& fontPath, unsigned int fontSize, unsigned int numRows, unsigned int maxWidth, const char* fmt, ...);
		~Text();

		void SetText(const std::string& text, const std::string& fontPath = std::string("keepFont"), int fontSize = -1, int numRows = -1);
		void SetText(const char* fmt, ...);
		void SetNumberOfRows(unsigned int numRows);
		void SetMaxWidth(unsigned int maxWidth);
		void SetTranslation(const glm::vec2& translation);
		void SetScale(const glm::vec2& scale);

		const std::string& GetText();
		unsigned int GetNumberOfRows();
		unsigned int GetMaxWidth();
		const glm::vec2& GetTranslation();
		const glm::vec2& GetScale();

		void Draw(const glm::mat4& projection, Shader& shader);
	};
}