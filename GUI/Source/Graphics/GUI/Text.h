#pragma once

#include "Font.h"

#include <vector>
#include <unordered_map>
#include <memory>
#include <glew.h>
#include <glm.hpp>
#include <string>
#include <cfloat>

namespace GUI {
	class Text {
	public:
		enum class HorizontalAlignment {
			LEFT = 1,
			CENTER = 2,
			RIGHT = 4,
			JUSTIFY = 8
		};

		enum class VerticalAlignment {
			TOP = 1,
			CENTER = 2,
			BOTTOM = 4
		};

		enum class Wrap {
			// LETTER wraps at the nearest letter
			LETTER = 1,
			// WORD wraps at the nearest word
			WORD = 2,
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

		unsigned int m_MaxNumberOfRows;
		unsigned int m_MaxWidth;
		HorizontalAlignment m_HorizontalAlignment;
		VerticalAlignment m_VerticalAlignment;
		Wrap m_Wrap;

		glm::vec4 m_Color;

		std::shared_ptr<Shader> m_Shader;

		std::string m_FontPath;

		int m_FontSize;

		bool m_ShouldRecalculate;

		float m_LineSpacing;

		unsigned int m_DPI;

		struct AlignedText {
			struct Row {
				std::string text;
				float min;
				float max;
				int numSpaces;
				// is true whenever the line ends in a space
				bool shouldJustify;

				Row()
					: text(), min(FLT_MAX), max(-FLT_MAX), numSpaces(0), shouldJustify(false)
				{

				}
			};

			std::vector<Row> rows;
			float min;
			float max;
			std::size_t numCharacters;



			AlignedText()
				: rows(), min(FLT_MAX), max(-FLT_MAX), numCharacters(0ull)
			{

			}
		};

		static Text::AlignedText Text::AlignText(const std::string& text, Font* font, unsigned int maxWidth, unsigned int maxLines, Wrap wrap);
		void Recalculate();
		static void PrepareText(const AlignedText& alignedText, Font* font, Font::Character::Face* vertices, Text::HorizontalAlignment horizontalAlignment, Text::VerticalAlignment verticalAlignment, float lineSpacing, glm::vec2& offset, glm::vec2& min, glm::vec2& max, glm::vec4& color);
		static void PrepareMarkdownText(const AlignedText& alignedText, Font* font, Font::Character::Face* vertices, Text::HorizontalAlignment horizontalAlignment, Text::VerticalAlignment verticalAlignment, float lineSpacing, glm::vec2& offset, glm::vec2& min, glm::vec2& max, glm::vec4& color);
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
		void SetDPI(unsigned int dpi);
		void SetSupportsMarkdown(bool supportsMarkdown);
		void SetHorizontalAlignment(HorizontalAlignment horizontalAlignment);
		void SetVerticalAlignment(VerticalAlignment verticalAlignment);
		void SetWrap(Wrap wrap);
		void SetMaxNumberOfRows(unsigned int maxNumRows);
		void SetMaxWidth(unsigned int maxWidth);
		void SetTranslation(const glm::vec2& translation);
		void SetScale(const glm::vec2& scale);
		void SetColor(const glm::vec4& color);
		void SetLineSpacing(float lineSpacing);

		const std::string& GetText();
		const std::string& GetFontPath();
		unsigned int GetFontSize();
		unsigned int GetDPI();
		bool GetSupportsMarkdown();
		HorizontalAlignment GetHorizontalAlignment();
		VerticalAlignment GetVerticalAlignment();
		Wrap GetWrap();
		unsigned int GetMaxNumberOfRows();
		unsigned int GetMaxWidth();
		const glm::vec2& GetTranslation();
		const glm::vec2& GetScale();
		const glm::vec4& GetColor();
		float GetLineSpacing();

		void Draw(const glm::mat4& projection);
	};
}