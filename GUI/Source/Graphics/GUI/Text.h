#pragma once

#include "Font.h"

#include <glew.h>
#include <glm.hpp>
#include <string>

namespace GUI {
	class Text {
	private:
		GLuint m_VAO;
		GLuint m_VBO;

		glm::vec2 m_Scale;
		glm::vec2 m_Translation;

		glm::vec2 m_Size;
		
		Font* m_Font;
		std::string m_Text;

		unsigned int m_NumberOfRows;
		unsigned int m_MaxWidth;
	public:
		Text(const std::string& fontPath, unsigned int fontSize, const std::string& text, unsigned int numRows, unsigned int maxWidth);
		~Text();

		void SetText(const std::string& newText, unsigned int fontSize);
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