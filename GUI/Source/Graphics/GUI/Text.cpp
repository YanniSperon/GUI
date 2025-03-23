#include "Text.h"
#include "FontManager.h"
#include "Console.h"
#include "MathExtensions.h"
#include "ShaderManager.h"

#include <cstdarg>
#include <glm/gtc/matrix_transform.hpp>

namespace GUI {
	Text::Text()
		: m_Fonts(), m_MaxNumberOfRows(0), m_MaxWidth(INT_MAX), m_Scale(1.0f, 1.0f), m_Translation(0.0f, 0.0f), m_Min(FLT_MAX, FLT_MAX), m_Max(-FLT_MAX, -FLT_MAX), m_SupportsMarkdown(false), m_HorizontalAlignment(HorizontalAlignment::LEFT), m_VerticalAlignment(VerticalAlignment::TOP), m_Color(1.0f, 1.0f, 1.0f, 1.0f), m_TranslationOffset(0.0f, 0.0f), m_Shader(ShaderManager::GetInstance()->GetShader("Resources/Shaders/Text", Shader::Type::Default)), m_FontPath(""), m_FontSize(12), m_ShouldRecalculate(false), m_LineSpacing(1.0f), m_Wrap(Text::Wrap::WORD), m_DPI(96)
	{
		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);

		glGenBuffers(1, &m_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(sizeof(GLfloat) * 2));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(sizeof(GLfloat) * 4));

		glFinish();
	}

	Text::~Text()
	{
		glDeleteBuffers(1, &m_VBO);
		glDeleteVertexArrays(1, &m_VAO);
		FontManager::GetInstance()->MarkForCleanup();
	}

	Text::AlignedText Text::AlignText(const std::string& text, Font* font, unsigned int maxWidth, unsigned int maxNumberOfRows, Wrap wrap) {
		
		std::map<char, Font::Character>& characters = font->GetCharacters();
		
		bool lastCharacterWasNewline = false;
		int kerningXPx;
		int kerningX;
		int characterBearingX;
		float baselineOffset;
		float shiftX;
		float baseline = 0.0f;
		float lastMax = 0.0f;
		float maxAtLastSpace = 0.0f;

		Text::AlignedText result;
		Text::AlignedText::Row currentRow;
		
		std::size_t rowStartIndex = 0;
		std::size_t lastSpaceIndex = 0;
		std::size_t rowSize = 0;
		
		unsigned int fontSize = font->GetSize();
		
		for (std::size_t i = 0; i < text.size(); ++i) {
			char c = text[i];

			if (result.rows.size() >= maxNumberOfRows) {
				break;
			}

			rowSize++;
			if (c == ' ') {
				currentRow.numSpaces++;
				lastSpaceIndex = i;
				maxAtLastSpace = lastMax;
				lastMax = 0.0f;
				if (rowSize == 1) {
					rowSize--;
					rowStartIndex++;
					continue;
				}
			}
			else if (c == '-') {
				maxAtLastSpace = lastMax;
				lastMax = 0.0f;
				lastSpaceIndex = i;
			}
			else if (c == '\n') {
				currentRow.text = text.substr(rowStartIndex, rowSize - 1ull);
				result.rows.push_back(currentRow);
				maxAtLastSpace = 0.0f;
				lastMax = 0.0f;
				lastSpaceIndex = i;
				rowStartIndex = i + 1ull;
				currentRow = Text::AlignedText::Row();
				baseline = 0.0f;
				rowSize = 0;
				continue;
			}

			Font::Character& ch = characters[c];
		
			// kerning, affects baseline
			kerningX = 0;
			if (i != 0) {
				Font::Character& previousChar = characters[text[i - 1]];
				if (previousChar.kerning.find(c) != previousChar.kerning.end()) {
					kerningX = previousChar.kerning[c].x;
				}
			}
			kerningXPx = (kerningX >> 6);
		
			// bearing, does not affect baseline
			characterBearingX = ch.bearing.x;
		
			// advance affects baseline so add it to bearing
			baselineOffset = kerningXPx + (ch.advance.x >> 6);

			// add together the final shift on the characters themselves
			shiftX = kerningXPx + characterBearingX + baseline;

			currentRow.min = glm::min(ch.face.bottomLeft.position.x + shiftX, currentRow.min);
			currentRow.max = glm::max(ch.face.topRight.position.x + shiftX, currentRow.max);

			if ((currentRow.max - currentRow.min) > maxWidth) {
				// reset the cursor
				baseline = 0.0f;
				if (lastSpaceIndex < rowStartIndex || wrap == Text::Wrap::LETTER) {
					currentRow.text = text.substr(rowStartIndex, i - rowStartIndex + 1ull);
				}
				else {
					currentRow.text = text.substr(rowStartIndex, rowSize - 1ull - (i - lastSpaceIndex));
					currentRow.numSpaces--;
					currentRow.max = maxAtLastSpace;
					i = lastSpaceIndex;
				}
				currentRow.shouldJustify = true;
				result.rows.push_back(currentRow);
				lastSpaceIndex = i;
				rowStartIndex = i + 1ull;
				currentRow = Text::AlignedText::Row();
				rowSize = 0;
				continue;
			}

			lastMax = currentRow.max;
			baseline += baselineOffset;
			
			lastCharacterWasNewline = false;
		}

		if (rowStartIndex != text.size() && result.rows.size() < maxNumberOfRows) {
			currentRow.text = text.substr(rowStartIndex);
			result.rows.push_back(currentRow);
		}
		for (std::size_t i = 0; i < result.rows.size(); ++i) {
			result.numCharacters += result.rows[i].text.size();
			result.min = glm::min(result.rows[i].min, result.min);
			result.max = glm::max(result.rows[i].max, result.max);
		}
		//if (maxWidth != INT_MAX) {
		//	result.max = INT_MAX;
		//}
		return result;
	}

	void Text::PrepareText(const AlignedText& alignedText, Font* font, Font::Character::Face* vertices, Text::HorizontalAlignment horizontalAlignment, Text::VerticalAlignment verticalAlignment, float lineSpacing, glm::vec2& offset, glm::vec2& min, glm::vec2& max, glm::vec4& color)
	{
		std::map<char, Font::Character>& characters = font->GetCharacters();
		
		int kerningXPx;
		int kerningX;
		int characterBearingX;
		float tempXOffset;
		float shiftX;
		std::size_t numCharactersAdded = 0ull;
		float tempIncrease = 0.0f;
		float justifySpaceSizeIncrease = 0.0f;
		float centerAlignIncrease = 0.0f;
		float rightAlignIncrease = 0.0f;
		float leftAlignIncrease = 0.0f;

		unsigned int fontSize = font->GetSize();
		float maxRowWidth = alignedText.max - alignedText.min;

		for (std::size_t i = 0; i < alignedText.rows.size(); ++i) {
			Text::AlignedText::Row row = alignedText.rows[i];
			offset.x -= row.min;
			if (horizontalAlignment == Text::HorizontalAlignment::JUSTIFY && row.shouldJustify) {
				justifySpaceSizeIncrease = ((maxRowWidth - (row.max - row.min)) / row.numSpaces);
			}
			else if (horizontalAlignment == Text::HorizontalAlignment::CENTER) {
				offset.x += (maxRowWidth / 2.0f) - ((row.max - row.min) / 2.0f);
			}
			else if (horizontalAlignment == Text::HorizontalAlignment::RIGHT) {
				offset.x += alignedText.max - row.max;
			}
			for (std::size_t j = 0; j < row.text.size(); ++j) {
				char c = row.text[j];

				Font::Character& ch = characters[c];

				if (c == ' ') {
					tempIncrease += justifySpaceSizeIncrease;
				}

				memcpy(&vertices[numCharactersAdded], &ch.face, sizeof(Font::Character::Face));

				kerningX = 0;

				if (j != 0) {
					Font::Character& previousChar = characters[row.text[j - 1]];
					if (previousChar.kerning.find(c) != previousChar.kerning.end()) {
						kerningX = previousChar.kerning[c].x;
					}
				}

				kerningXPx = (kerningX >> 6);

				characterBearingX = ch.bearing.x;

				tempXOffset = kerningXPx + (ch.advance.x >> 6) + tempIncrease;

				shiftX = kerningXPx + characterBearingX + offset.x;
				
				vertices[numCharactersAdded].bottomLeft.position.x += shiftX;
				vertices[numCharactersAdded].bottomLeft.position.y -= offset.y;
				vertices[numCharactersAdded].bottomLeft.color.r = color.r;
				vertices[numCharactersAdded].bottomLeft.color.g = color.g;
				vertices[numCharactersAdded].bottomLeft.color.b = color.b;
				vertices[numCharactersAdded].bottomLeft.color.a = color.a;

				vertices[numCharactersAdded].topLeft.position.x += shiftX;
				vertices[numCharactersAdded].topLeft.position.y -= offset.y;
				vertices[numCharactersAdded].topLeft.color.r = color.r;
				vertices[numCharactersAdded].topLeft.color.g = color.g;
				vertices[numCharactersAdded].topLeft.color.b = color.b;
				vertices[numCharactersAdded].topLeft.color.a = color.a;

				vertices[numCharactersAdded].topRight.position.x += shiftX;
				vertices[numCharactersAdded].topRight.position.y -= offset.y;
				vertices[numCharactersAdded].topRight.color.r = color.r;
				vertices[numCharactersAdded].topRight.color.g = color.g;
				vertices[numCharactersAdded].topRight.color.b = color.b;
				vertices[numCharactersAdded].topRight.color.a = color.a;

				vertices[numCharactersAdded].bottomLeftDuplicate.position.x += shiftX;
				vertices[numCharactersAdded].bottomLeftDuplicate.position.y -= offset.y;
				vertices[numCharactersAdded].bottomLeftDuplicate.color.r = color.r;
				vertices[numCharactersAdded].bottomLeftDuplicate.color.g = color.g;
				vertices[numCharactersAdded].bottomLeftDuplicate.color.b = color.b;
				vertices[numCharactersAdded].bottomLeftDuplicate.color.a = color.a;

				vertices[numCharactersAdded].topRightDuplicate.position.x += shiftX;
				vertices[numCharactersAdded].topRightDuplicate.position.y -= offset.y;
				vertices[numCharactersAdded].topRightDuplicate.color.r = color.r;
				vertices[numCharactersAdded].topRightDuplicate.color.g = color.g;
				vertices[numCharactersAdded].topRightDuplicate.color.b = color.b;
				vertices[numCharactersAdded].topRightDuplicate.color.a = color.a;

				vertices[numCharactersAdded].bottomRight.position.x += shiftX;
				vertices[numCharactersAdded].bottomRight.position.y -= offset.y;
				vertices[numCharactersAdded].bottomRight.color.r = color.r;
				vertices[numCharactersAdded].bottomRight.color.g = color.g;
				vertices[numCharactersAdded].bottomRight.color.b = color.b;
				vertices[numCharactersAdded].bottomRight.color.a = color.a;

				min.x = glm::min(vertices[numCharactersAdded].bottomLeft.position.x, min.x);
				min.y = glm::min(vertices[numCharactersAdded].bottomLeft.position.y, min.y);

				max.x = glm::max(vertices[numCharactersAdded].topRight.position.x, max.x);
				max.y = glm::max(vertices[numCharactersAdded].topRight.position.y, max.y);
				offset.x += tempXOffset;
				tempIncrease = 0.0f;
				numCharactersAdded++;
			}
			offset.x = 0.0f;
			offset.y += fontSize * lineSpacing;
		}
	}

#define WEIGHT_BOLD 1
#define WEIGHT_ITALIC 2
#define SIZE_2_00X 4
#define SIZE_1_50X 8
#define SIZE_1_17X 16
#define SIZE_1_00X 32
#define SIZE_0_83X 64
#define SIZE_0_67X 128

	class MarkdownData {
	public:
		int m_Effect;
		std::string m_Limiter;
		std::string m_Delimiter;
	
		MarkdownData(int effect, const std::string& limiter, const std::string& delimiter)
			: m_Effect(effect), m_Limiter(limiter), m_Delimiter(delimiter)
		{
			
		}
	};

	void Text::PrepareMarkdownText(const AlignedText& alignedText, Font* font, Font::Character::Face* vertices, Text::HorizontalAlignment horizontalAlignment, Text::VerticalAlignment verticalAlignment, float lineSpacing, glm::vec2& offset, glm::vec2& min, glm::vec2& max, glm::vec4& color)
	{
		//std::vector<MarkdownData> formatting = std::vector<MarkdownData>();
		
		//PrepareText(m_Text, m_Fonts[SIZE_2_00X | WEIGHT_BOLD | WEIGHT_ITALIC].get(), 1.0f, vertices, m_MaxWidth, cursor, m_Min, m_Max, m_Color);
		
		//for (unsigned int i = 0; i < text.size(); ++i) {
		//
		//}


	}

	void Text::Recalculate()
	{
		if (m_FontPath != "") {
			m_Min = glm::vec2(FLT_MAX, FLT_MAX);
			m_Max = glm::vec2(-FLT_MAX, -FLT_MAX);

			//if (m_SupportsMarkdown) {
				//m_Fonts.clear();
				//
				//std::string defaultFontPath = m_FontPath + ".ttf";
				//std::string boldFontPath = m_FontPath + "bd.ttf";
				//std::string italicFontPath = m_FontPath + "i.ttf";
				//std::string boldItalicFontPath = m_FontPath + "bi.ttf";
				//
				//// h1
				//unsigned int fontSize_2_00X = glm::round(m_FontSize * 2.0f);
				//m_Fonts[SIZE_2_00X] = FontManager::GetInstance()->GetFont(defaultFontPath, fontSize_2_00X, m_DPI);
				//m_Fonts[SIZE_2_00X | WEIGHT_BOLD] = FontManager::GetInstance()->GetFont(boldFontPath, fontSize_2_00X, m_DPI);
				//m_Fonts[SIZE_2_00X | WEIGHT_ITALIC] = FontManager::GetInstance()->GetFont(italicFontPath, fontSize_2_00X, m_DPI);
				//m_Fonts[SIZE_2_00X | WEIGHT_BOLD | WEIGHT_ITALIC] = FontManager::GetInstance()->GetFont(boldItalicFontPath, fontSize_2_00X, m_DPI);
				//
				//// h2
				//unsigned int fontSize_1_50X = glm::round(m_FontSize * 1.5f);
				//m_Fonts[SIZE_1_50X] = FontManager::GetInstance()->GetFont(defaultFontPath, fontSize_1_50X, m_DPI);
				//m_Fonts[SIZE_1_50X | WEIGHT_BOLD] = FontManager::GetInstance()->GetFont(boldFontPath, fontSize_1_50X, m_DPI);
				//m_Fonts[SIZE_1_50X | WEIGHT_ITALIC] = FontManager::GetInstance()->GetFont(italicFontPath, fontSize_1_50X, m_DPI);
				//m_Fonts[SIZE_1_50X | WEIGHT_BOLD | WEIGHT_ITALIC] = FontManager::GetInstance()->GetFont(boldItalicFontPath, fontSize_1_50X, m_DPI);
				//
				//// h3
				//unsigned int fontSize_1_17X = glm::round(m_FontSize * 1.17f);
				//m_Fonts[SIZE_1_17X] = FontManager::GetInstance()->GetFont(defaultFontPath, fontSize_1_17X, m_DPI);
				//m_Fonts[SIZE_1_17X | WEIGHT_BOLD] = FontManager::GetInstance()->GetFont(boldFontPath, fontSize_1_17X, m_DPI);
				//m_Fonts[SIZE_1_17X | WEIGHT_ITALIC] = FontManager::GetInstance()->GetFont(italicFontPath, fontSize_1_17X, m_DPI);
				//m_Fonts[SIZE_1_17X | WEIGHT_BOLD | WEIGHT_ITALIC] = FontManager::GetInstance()->GetFont(boldItalicFontPath, fontSize_1_17X, m_DPI);
				//
				//// h4
				//unsigned int fontSize_1_00X = m_FontSize;
				//m_Fonts[SIZE_1_00X] = FontManager::GetInstance()->GetFont(defaultFontPath, fontSize_1_00X, m_DPI);
				//m_Fonts[SIZE_1_00X | WEIGHT_BOLD] = FontManager::GetInstance()->GetFont(boldFontPath, fontSize_1_00X, m_DPI);
				//m_Fonts[SIZE_1_00X | WEIGHT_ITALIC] = FontManager::GetInstance()->GetFont(italicFontPath, fontSize_1_00X, m_DPI);
				//m_Fonts[SIZE_1_00X | WEIGHT_BOLD | WEIGHT_ITALIC] = FontManager::GetInstance()->GetFont(boldItalicFontPath, fontSize_1_00X, m_DPI);
				//
				//// h5
				//unsigned int fontSize_0_83X = glm::round(m_FontSize * 0.83f);
				//m_Fonts[SIZE_0_83X] = FontManager::GetInstance()->GetFont(defaultFontPath, fontSize_0_83X, m_DPI);
				//m_Fonts[SIZE_0_83X | WEIGHT_BOLD] = FontManager::GetInstance()->GetFont(boldFontPath, fontSize_0_83X, m_DPI);
				//m_Fonts[SIZE_0_83X | WEIGHT_ITALIC] = FontManager::GetInstance()->GetFont(italicFontPath, fontSize_0_83X, m_DPI);
				//m_Fonts[SIZE_0_83X | WEIGHT_BOLD | WEIGHT_ITALIC] = FontManager::GetInstance()->GetFont(boldItalicFontPath, fontSize_0_83X, m_DPI);
				//
				//// h6
				//unsigned int fontSize_0_67X = glm::round(m_FontSize * 0.67f);
				//m_Fonts[SIZE_0_67X] = FontManager::GetInstance()->GetFont(defaultFontPath, fontSize_0_67X, m_DPI);
				//m_Fonts[SIZE_0_67X | WEIGHT_BOLD] = FontManager::GetInstance()->GetFont(boldFontPath, fontSize_0_67X, m_DPI);
				//m_Fonts[SIZE_0_67X | WEIGHT_ITALIC] = FontManager::GetInstance()->GetFont(italicFontPath, fontSize_0_67X, m_DPI);
				//m_Fonts[SIZE_0_67X | WEIGHT_BOLD | WEIGHT_ITALIC] = FontManager::GetInstance()->GetFont(boldItalicFontPath, fontSize_0_67X, m_DPI);
				//
				//FontManager::GetInstance()->MarkForCleanup();
				//
				//PrepareMarkdownText(characterFaces);
				//
				//Console::Error("Markdown text is not yet setup");
			//}
			//else {

			m_Fonts.clear();

			std::string defaultFontPath = m_FontPath + ".ttf";

			m_Fonts[SIZE_1_00X] = FontManager::GetInstance()->GetFont(defaultFontPath, m_FontSize, m_DPI);

			FontManager::GetInstance()->MarkForCleanup();

			Text::AlignedText alignedText = Text::AlignText(m_Text, m_Fonts[SIZE_1_00X].get(), m_MaxWidth, m_MaxNumberOfRows, m_Wrap);
			
			GUI::Font::Character::Face* characterFaces = new GUI::Font::Character::Face[alignedText.numCharacters];

			PrepareText(alignedText, m_Fonts[SIZE_1_00X].get(), characterFaces, m_HorizontalAlignment, m_VerticalAlignment, m_LineSpacing, glm::vec2(0.0f, 0.0f), m_Min, m_Max, m_Color);
			m_TranslationOffset = glm::round(m_Max - ((glm::abs(m_Min) + glm::abs(m_Max)) * 0.5f));
			//}
			//m_TranslationOffset = glm::round(m_Max - ((glm::abs(m_Min) + glm::abs(m_Max)) * 0.5f));
			//m_TranslationOffset = glm::vec2(0.0f, glm::round(m_Max.y - ((glm::abs(m_Min.y) + glm::abs(m_Max.y)) * 0.5f)));

			glGenVertexArrays(1, &m_VAO);
			glBindVertexArray(m_VAO);

			glGenBuffers(1, &m_VBO);
			glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
			glBufferData(GL_ARRAY_BUFFER, alignedText.numCharacters * sizeof(GUI::Font::Character::Face), characterFaces, GL_DYNAMIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(sizeof(GLfloat) * 2));
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(sizeof(GLfloat) * 4));

			glFinish();

			delete[] characterFaces;
		}
		m_ShouldRecalculate = false;
	}

	void Text::SetText(const char* fmt, ...)
	{
		va_list args, argsCopy;
		va_start(args, fmt);
		va_copy(argsCopy, args);
		int bufferSize = vsnprintf(nullptr, 0, fmt, argsCopy) + 1;
		char* message = new char[bufferSize];
		vsnprintf(message, bufferSize, fmt, args);
		m_Text = std::string(message);
		va_end(args);
		va_end(argsCopy);
		delete[] message;

		std::string replacementSpaces = "   ";

		for (std::size_t i = 0; i < m_Text.size(); ++i) {
			if (m_Text[i] == '\t') {
				m_Text[i] = ' ';
				m_Text.insert(i, replacementSpaces);
			}
		}

		m_ShouldRecalculate = true;
	}

	void Text::SetText(const std::string& text)
	{
		m_Text = text;

		std::string replacementSpaces = "   ";

		for (std::size_t i = 0; i < m_Text.size(); ++i) {
			if (m_Text[i] == '\t') {
				m_Text[i] = ' ';
				m_Text.insert(i, replacementSpaces);
			}
		}

		m_ShouldRecalculate = true;
	}

	void Text::SetFont(const std::string& path)
	{
		m_ShouldRecalculate = (m_FontPath != path) || m_ShouldRecalculate;
		m_FontPath = path;
	}

	void Text::SetFontSize(unsigned int size)
	{
		m_ShouldRecalculate = (m_FontSize != size) || m_ShouldRecalculate;
		m_FontSize = size;
	}

	void Text::SetDPI(unsigned int dpi)
	{
		m_ShouldRecalculate = (m_DPI != dpi) || m_ShouldRecalculate;
		m_DPI = dpi;
	}

	void Text::SetSupportsMarkdown(bool supportsMarkdown)
	{
		m_ShouldRecalculate = (m_SupportsMarkdown != supportsMarkdown) || m_ShouldRecalculate;
		m_SupportsMarkdown = supportsMarkdown;
	}

	void Text::SetHorizontalAlignment(HorizontalAlignment horizontalAlignment)
	{
		m_ShouldRecalculate = (m_HorizontalAlignment != horizontalAlignment) || m_ShouldRecalculate;
		m_HorizontalAlignment = horizontalAlignment;
	}

	void Text::SetVerticalAlignment(VerticalAlignment verticalAlignment)
	{
		m_ShouldRecalculate = (m_VerticalAlignment != verticalAlignment) || m_ShouldRecalculate;
		m_VerticalAlignment = verticalAlignment;
	}

	void Text::SetWrap(Wrap wrap)
	{
		m_ShouldRecalculate = (m_Wrap != wrap) || m_ShouldRecalculate;
		m_Wrap = wrap;
	}

	void Text::SetMaxNumberOfRows(unsigned int maxNumRows)
	{
		unsigned int tempMaxRows = maxNumRows == 0 ? INT_MAX : maxNumRows;
		m_ShouldRecalculate = (m_MaxNumberOfRows != tempMaxRows) || m_ShouldRecalculate;
		m_MaxNumberOfRows = tempMaxRows;
	}

	void Text::SetMaxWidth(unsigned int maxWidth)
	{
		unsigned int tempMaxWidth = maxWidth == 0 ? INT_MAX : maxWidth;
		m_ShouldRecalculate = (m_MaxWidth != tempMaxWidth) || m_ShouldRecalculate;
		m_MaxWidth = tempMaxWidth;
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
		m_ShouldRecalculate = (m_Color != color) || m_ShouldRecalculate;
		m_Color = color;
	}

	void Text::SetLineSpacing(float lineSpacing)
	{
		m_LineSpacing = lineSpacing;
	}

	const std::string& Text::GetText()
	{
		return m_Text;
	}

	const std::string& Text::GetFontPath()
	{
		return m_FontPath;
	}

	unsigned int Text::GetFontSize()
	{
		return m_FontSize;
	}

	unsigned int Text::GetDPI()
	{
		return m_DPI;
	}

	bool Text::GetSupportsMarkdown()
	{
		return m_SupportsMarkdown;
	}

	Text::HorizontalAlignment Text::GetHorizontalAlignment()
	{
		return m_HorizontalAlignment;
	}

	Text::VerticalAlignment Text::GetVerticalAlignment()
	{
		return m_VerticalAlignment;
	}

	Text::Wrap Text::GetWrap()
	{
		return m_Wrap;
	}

	unsigned int Text::GetMaxNumberOfRows()
	{
		return m_MaxNumberOfRows;
	}

	unsigned int Text::GetMaxWidth()
	{
		return m_MaxWidth == INT_MAX ? 0 : m_MaxWidth;
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

	float Text::GetLineSpacing()
	{
		return m_LineSpacing;
	}

	void Text::Draw(const glm::mat4& projection)
	{
		if (m_ShouldRecalculate) {
			Recalculate();
		}

		if (m_SupportsMarkdown) {
			m_Shader->Bind();
			m_Shader->SetMat4("u_P", projection);
			m_Shader->SetMat4("u_M", glm::translate(glm::mat4(1.0f), glm::vec3(m_Translation - m_TranslationOffset, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(m_Scale, 1.0f)));
			glActiveTexture(GL_TEXTURE0);
			GLuint textureID = 0;
			if (m_Fonts.size() > 0) {
				textureID = m_Fonts[SIZE_2_00X]->GetAtlasTextureID();
			}
			glBindTexture(GL_TEXTURE_2D, textureID);
			glBindVertexArray(m_VAO);
			glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
			glDrawArrays(GL_TRIANGLES, 0, 6 * m_Text.length());
			//glDrawArrays(GL_TRIANGLE_STRIPS, 0, 4 * m_Text.length());
		}
		else {
			m_Shader->Bind();
			m_Shader->SetMat4("u_P", projection);
			m_Shader->SetMat4("u_M", glm::translate(glm::mat4(1.0f), glm::vec3(m_Translation - m_TranslationOffset, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(m_Scale, 1.0f)));
			glActiveTexture(GL_TEXTURE0);
			GLuint textureID = 0;
			if (m_Fonts.size() > 0) {
				textureID = m_Fonts[SIZE_1_00X]->GetAtlasTextureID();
			}
			glBindTexture(GL_TEXTURE_2D, textureID);
			glBindVertexArray(m_VAO);
			glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
			glDrawArrays(GL_TRIANGLES, 0, 6 * m_Text.length());
		}
	}
}