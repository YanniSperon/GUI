#pragma once

#include "Font.h"

#include <vector>
#include <unordered_map>
#include <memory>
#include <glew.h>
#include <glm.hpp>
#include <string>

namespace GUI {
	class Text {
	public:
		enum class Alignment {
			DEFAULT = 1,
			PREVIOUS = 2,
			LEFT = 4,
			CENTER = 8,
			RIGHT = 16
		};
	private:
		bool m_SupportsMarkdown;

		GLuint m_VAO;
		GLuint m_VBO;
		glm::vec2 m_TranslationOffset;

		glm::vec2 m_Max;
		glm::vec2 m_Min;

		std::unordered_map<unsigned int, std::shared_ptr<Font>> m_Fonts;
		std::string m_Text;

		unsigned int m_NumberOfRows;
		unsigned int m_MaxWidth;
		Alignment m_Alignment;

		glm::vec4 m_Color;

		std::shared_ptr<Shader> m_Shader;

		std::string m_FontPath;

		bool m_UsePtSize;

		int m_FontSize;

		bool m_ShouldRecalculate;

		float m_LineSpacing;

		void PrepareMarkdownText(Character::Face* vertices);
		void Recalculate();
	protected:
		glm::vec2 m_Scale;
		glm::vec2 m_Translation;
	public:
		Text();
		~Text();

		void SetText(const char* fmt, ...);
		void SetText(const std::string& text);
		void SetFont(const std::string& path);
		void SetFontSize(unsigned int size);
		void SetUsePtSize(bool usePtSize);
		void SetSupportsMarkdown(bool supportsMarkdown);
		void SetAlignment(Alignment alignment);
		void SetNumberOfRows(unsigned int numRows);
		void SetMaxWidth(unsigned int maxWidth);
		void SetTranslation(const glm::vec2& translation);
		void SetScale(const glm::vec2& scale);
		void SetColor(const glm::vec4& color);
		void SetLineSpacing(float lineSpacing);

		const std::string& GetText();
		const std::string& GetFontPath();
		unsigned int GetFontSize();
		bool GetUsePtSize();
		bool GetSupportsMarkdown();
		Alignment GetAlignment();
		unsigned int GetNumberOfRows();
		unsigned int GetMaxWidth();
		const glm::vec2& GetTranslation();
		const glm::vec2& GetScale();
		const glm::vec4& GetColor();
		float GetLineSpacing();

		void Draw(const glm::mat4& projection);
	};
}