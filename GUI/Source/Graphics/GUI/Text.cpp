#include "Text.h"
#include "FontManager.h"
#include "Console.h"
#include "MathExtensions.h"
#include "ShaderManager.h"

#include <cstdarg>
#include <glm/gtc/matrix_transform.hpp>

namespace GUI {
	Text::Text()
		: m_Fonts(), m_NumberOfRows(0), m_MaxWidth(INT_MAX), m_Scale(1.0f, 1.0f), m_Translation(0.0f, 0.0f), m_Min(FLT_MAX, FLT_MAX), m_Max(-FLT_MAX, -FLT_MAX), m_SupportsMarkdown(false), m_Alignment(Alignment::LEFT), m_Color(1.0f, 1.0f, 1.0f, 1.0f), m_TranslationOffset(0.0f, 0.0f), m_Shader(ShaderManager::GetInstance()->GetShader("Resources/Shaders/Text", SHADER_VERTEX_SHADER | SHADER_FRAGMENT_SHADER)), m_FontPath(""), m_FontSize(12), m_UsePtSize(true), m_ShouldRecalculate(false), m_LineSpacing(1.0f)
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

	std::string AlignText(const std::string& text, Font* font, unsigned int maxWidth, unsigned int maxLines, glm::vec2& min, glm::vec2& max) {
		//glm::vec2 tempOffset = glm::vec2(0.0f, 0.0f);
		//
		//std::map<char, Character>& characters = font->GetCharacters();
		//
		//int kerningXPx = 0;
		//int kerningX = 0;
		//int characterBearingX = 0;
		//float tempXOffset = 0.0f;
		//float shiftX = 0.0f;
		//glm::vec2 currMin = glm::vec2(0.0f, 0.0f);
		//glm::vec2 currMax = glm::vec2(0.0f, 0.0f);
		//
		//unsigned int fontSize = font->GetSize();
		//
		//int linesCount = 0;
		//int charsOnLineCount = 0;
		//
		//for (int i = 0; i < text.size(); i++) {
		//	char c = text[i];
		//	if (c == '\n') {
		//		tempOffset.x = 0.0f;
		//		tempOffset.y += fontSize;
		//		linesCount++;
		//		continue;
		//	}
		//
		//	Character& ch = characters[c];
		//
		//	kerningX = 0;
		//
		//	if (i != 0) {
		//		Character& previousChar = characters[text[i - 1]];
		//		if (previousChar.kerning.find(c) != previousChar.kerning.end()) {
		//			kerningX = previousChar.kerning[c].x;
		//		}
		//	}
		//
		//	kerningXPx = (kerningX >> 6);
		//
		//	characterBearingX = ch.bearing.x;
		//
		//	tempXOffset = kerningXPx + (ch.advance.x >> 6);
		//	if (tempOffset.x + tempXOffset > maxWidth) {
		//		tempOffset.x = 0.0f;
		//		tempOffset.y += fontSize;
		//		kerningXPx = 0;
		//		characterBearingX = 0;
		//		linesCount++;
		//	}
		//
		//	shiftX = kerningXPx + characterBearingX + tempOffset.x;
		//
		//	currMin.x = ch.face.bottomLeft.position.x + shiftX;
		//	currMin.x = ch.face.bottomLeft.position.y - tempOffset.y;
		//	min.x = glm::min(currMin.x, min.x);
		//	min.y = glm::min(currMin.y, min.y);
		//	currMax.x = (ch.face.topRight.position.x + shiftX);
		//	currMax.y = ch.face.topRight.position.y - tempOffset.y;
		//	max.x = glm::max(currMax.x, max.x);
		//	max.y = glm::max(currMax.y, max.y);
		//
		//	tempOffset.x += tempXOffset;
		//	charsOnLineCount++;
		//}
		return text;
	}

	void PrepareText(const std::string& text, float lineSpacing, Font* font, Character::Face* vertices, unsigned int maxWidth, glm::vec2& offset, glm::vec2& min, glm::vec2& max, glm::vec4& color)
	{
		std::map<char, Character>& characters = font->GetCharacters();
		
		int kerningXPx;
		int kerningX;
		int characterBearingX;
		float tempXOffset;
		float shiftX;
		glm::vec2 currMin;
		glm::vec2 currMax;

		unsigned int fontSize = font->GetSize();

		for (std::size_t i = 0; i < text.size(); ++i) {
			char c = text[i];
			if (c == '\n') {
				offset.x = 0.0f;
				offset.y += fontSize;
				continue;
			}
			
			Character& ch = characters[c];

			memcpy(&vertices[i], &ch.face, sizeof(Character::Face));
		
			kerningX = 0;
		
			if (i != 0) {
				Character& previousChar = characters[text[i - 1]];
				if (previousChar.kerning.find(c) != previousChar.kerning.end()) {
					kerningX = previousChar.kerning[c].x;
				}
			}
			
			kerningXPx = (kerningX >> 6);

			characterBearingX = ch.bearing.x;
		
			tempXOffset = kerningXPx + (ch.advance.x >> 6);
			if (offset.x + tempXOffset > maxWidth) {
				offset.x = 0.0f;
				offset.y += fontSize * lineSpacing;
				kerningXPx = 0;
				characterBearingX = 0;
			}
		
			shiftX = kerningXPx + characterBearingX + offset.x;

			vertices[i].bottomLeft.position.x += shiftX;
			vertices[i].bottomLeft.position.y -= offset.y;
			vertices[i].bottomLeft.color.r = color.r;
			vertices[i].bottomLeft.color.g = color.g;
			vertices[i].bottomLeft.color.b = color.b;
			vertices[i].bottomLeft.color.a = color.a;

			vertices[i].topLeft.position.x += shiftX;
			vertices[i].topLeft.position.y -= offset.y;
			vertices[i].topLeft.color.r = color.r;
			vertices[i].topLeft.color.g = color.g;
			vertices[i].topLeft.color.b = color.b;
			vertices[i].topLeft.color.a = color.a;

			vertices[i].topRight.position.x += shiftX;
			vertices[i].topRight.position.y -= offset.y;
			vertices[i].topRight.color.r = color.r;
			vertices[i].topRight.color.g = color.g;
			vertices[i].topRight.color.b = color.b;
			vertices[i].topRight.color.a = color.a;

			vertices[i].bottomLeftDuplicate.position.x += shiftX;
			vertices[i].bottomLeftDuplicate.position.y -= offset.y;
			vertices[i].bottomLeftDuplicate.color.r = color.r;
			vertices[i].bottomLeftDuplicate.color.g = color.g;
			vertices[i].bottomLeftDuplicate.color.b = color.b;
			vertices[i].bottomLeftDuplicate.color.a = color.a;

			vertices[i].topRightDuplicate.position.x += shiftX;
			vertices[i].topRightDuplicate.position.y -= offset.y;
			vertices[i].topRightDuplicate.color.r = color.r;
			vertices[i].topRightDuplicate.color.g = color.g;
			vertices[i].topRightDuplicate.color.b = color.b;
			vertices[i].topRightDuplicate.color.a = color.a;

			vertices[i].bottomRight.position.x += shiftX;
			vertices[i].bottomRight.position.y -= offset.y;
			vertices[i].bottomRight.color.r = color.r;
			vertices[i].bottomRight.color.g = color.g;
			vertices[i].bottomRight.color.b = color.b;
			vertices[i].bottomRight.color.a = color.a;

			currMin.x = vertices[i].bottomLeft.position.x;
			currMin.y = vertices[i].bottomLeft.position.y;
			min.x = glm::min(currMin.x, min.x);
			min.y = glm::min(currMin.y, min.y);
			currMax.x = vertices[i].topRight.position.x;
			currMax.y = vertices[i].topRight.position.y;
			max.x = glm::max(currMax.x, max.x);
			max.y = glm::max(currMax.y, max.y);
			offset.x += tempXOffset;
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

	std::vector<std::pair<std::string, float>> AlignText(const std::string& text, Font* font, unsigned int maxWidth, Text::Alignment alignment)
	{
		// string and where the start position should be
		std::vector<std::pair<std::string, float>> lines;

		std::map<char, Character>& characters = font->GetCharacters();

		int kerningXPx = 0;
		int kerningX = 0;
		float tempXOffset = 0.0f;
		glm::vec2 currMin;
		glm::vec2 currMax;
		float offset = 0.0f;

		unsigned int fontSize = font->GetSize();

		std::string currentLine = "";

		// check if 

		for (std::size_t i = 0; i < text.size(); ++i) {
			char c = text[i];

			// check if newline
			if (c == '\n') {
				offset = 0.0f;
				continue;
			}

			Character& ch = characters[c];

			// calculate kerning
			kerningX = 0;
			if (currentLine.size() != 0) {
				Character& previousChar = characters[text[i - 1]];
				if (previousChar.kerning.find(c) != previousChar.kerning.end()) {
					kerningX = previousChar.kerning[c].x;
				}
			}

			// kerning + advance
			tempXOffset = (kerningX >> 6) + (ch.advance.x >> 6);
			
			// check if offset + kerning + advance goes out of bounds
			if (offset + tempXOffset > maxWidth) {
				offset = 0.0f;
			}
			else {
				offset += tempXOffset;
			}
		}
		return lines;
	}

	void Text::PrepareMarkdownText(Character::Face* vertices)
	{
		std::vector<MarkdownData> formatting = std::vector<MarkdownData>();
		
		glm::vec2 cursor = glm::vec2(0.0f, 0.0f);

		//PrepareText(m_Text, m_Fonts[SIZE_2_00X | WEIGHT_BOLD | WEIGHT_ITALIC].get(), 1.0f, vertices, m_MaxWidth, cursor, m_Min, m_Max, m_Color);
		PrepareText(m_Text, m_LineSpacing, m_Fonts[SIZE_2_00X].get(), vertices, m_MaxWidth, cursor, m_Min, m_Max, m_Color);
		//for (unsigned int i = 0; i < text.size(); ++i) {
		//
		//}
	}

	void Text::Recalculate()
	{
		if (m_FontPath != "") {
			m_Min = glm::vec2(FLT_MAX, FLT_MAX);
			m_Max = glm::vec2(-FLT_MAX, -FLT_MAX);

			unsigned int stringLength = m_Text.length();

			GUI::Character::Face* characterFaces = new GUI::Character::Face[stringLength];

			if (m_Alignment != Text::Alignment::LEFT) {
				Console::Warning("Text does not currently support non-left alignments!");
			}

			if (m_SupportsMarkdown) {
				m_Fonts.clear();

				std::string defaultFontPath = m_FontPath + ".ttf";
				std::string boldFontPath = m_FontPath + "bd.ttf";
				std::string italicFontPath = m_FontPath + "i.ttf";
				std::string boldItalicFontPath = m_FontPath + "bi.ttf";

				// h1
				unsigned int fontSize_2_00X = glm::round(m_FontSize * 2.0f);
				m_Fonts[SIZE_2_00X] = FontManager::GetInstance()->GetFont(defaultFontPath, fontSize_2_00X, m_UsePtSize);
				m_Fonts[SIZE_2_00X | WEIGHT_BOLD] = FontManager::GetInstance()->GetFont(boldFontPath, fontSize_2_00X, m_UsePtSize);
				m_Fonts[SIZE_2_00X | WEIGHT_ITALIC] = FontManager::GetInstance()->GetFont(italicFontPath, fontSize_2_00X, m_UsePtSize);
				m_Fonts[SIZE_2_00X | WEIGHT_BOLD | WEIGHT_ITALIC] = FontManager::GetInstance()->GetFont(boldItalicFontPath, fontSize_2_00X, m_UsePtSize);

				// h2
				unsigned int fontSize_1_50X = glm::round(m_FontSize * 1.5f);
				m_Fonts[SIZE_1_50X] = FontManager::GetInstance()->GetFont(defaultFontPath, fontSize_1_50X, m_UsePtSize);
				m_Fonts[SIZE_1_50X | WEIGHT_BOLD] = FontManager::GetInstance()->GetFont(boldFontPath, fontSize_1_50X, m_UsePtSize);
				m_Fonts[SIZE_1_50X | WEIGHT_ITALIC] = FontManager::GetInstance()->GetFont(italicFontPath, fontSize_1_50X, m_UsePtSize);
				m_Fonts[SIZE_1_50X | WEIGHT_BOLD | WEIGHT_ITALIC] = FontManager::GetInstance()->GetFont(boldItalicFontPath, fontSize_1_50X, m_UsePtSize);

				// h3
				unsigned int fontSize_1_17X = glm::round(m_FontSize * 1.17f);
				m_Fonts[SIZE_1_17X] = FontManager::GetInstance()->GetFont(defaultFontPath, fontSize_1_17X, m_UsePtSize);
				m_Fonts[SIZE_1_17X | WEIGHT_BOLD] = FontManager::GetInstance()->GetFont(boldFontPath, fontSize_1_17X, m_UsePtSize);
				m_Fonts[SIZE_1_17X | WEIGHT_ITALIC] = FontManager::GetInstance()->GetFont(italicFontPath, fontSize_1_17X, m_UsePtSize);
				m_Fonts[SIZE_1_17X | WEIGHT_BOLD | WEIGHT_ITALIC] = FontManager::GetInstance()->GetFont(boldItalicFontPath, fontSize_1_17X, m_UsePtSize);

				// h4
				unsigned int fontSize_1_00X = m_FontSize;
				m_Fonts[SIZE_1_00X] = FontManager::GetInstance()->GetFont(defaultFontPath, fontSize_1_00X, m_UsePtSize);
				m_Fonts[SIZE_1_00X | WEIGHT_BOLD] = FontManager::GetInstance()->GetFont(boldFontPath, fontSize_1_00X, m_UsePtSize);
				m_Fonts[SIZE_1_00X | WEIGHT_ITALIC] = FontManager::GetInstance()->GetFont(italicFontPath, fontSize_1_00X, m_UsePtSize);
				m_Fonts[SIZE_1_00X | WEIGHT_BOLD | WEIGHT_ITALIC] = FontManager::GetInstance()->GetFont(boldItalicFontPath, fontSize_1_00X, m_UsePtSize);

				// h5
				unsigned int fontSize_0_83X = glm::round(m_FontSize * 0.83f);
				m_Fonts[SIZE_0_83X] = FontManager::GetInstance()->GetFont(defaultFontPath, fontSize_0_83X, m_UsePtSize);
				m_Fonts[SIZE_0_83X | WEIGHT_BOLD] = FontManager::GetInstance()->GetFont(boldFontPath, fontSize_0_83X, m_UsePtSize);
				m_Fonts[SIZE_0_83X | WEIGHT_ITALIC] = FontManager::GetInstance()->GetFont(italicFontPath, fontSize_0_83X, m_UsePtSize);
				m_Fonts[SIZE_0_83X | WEIGHT_BOLD | WEIGHT_ITALIC] = FontManager::GetInstance()->GetFont(boldItalicFontPath, fontSize_0_83X, m_UsePtSize);

				// h6
				unsigned int fontSize_0_67X = glm::round(m_FontSize * 0.67f);
				m_Fonts[SIZE_0_67X] = FontManager::GetInstance()->GetFont(defaultFontPath, fontSize_0_67X, m_UsePtSize);
				m_Fonts[SIZE_0_67X | WEIGHT_BOLD] = FontManager::GetInstance()->GetFont(boldFontPath, fontSize_0_67X, m_UsePtSize);
				m_Fonts[SIZE_0_67X | WEIGHT_ITALIC] = FontManager::GetInstance()->GetFont(italicFontPath, fontSize_0_67X, m_UsePtSize);
				m_Fonts[SIZE_0_67X | WEIGHT_BOLD | WEIGHT_ITALIC] = FontManager::GetInstance()->GetFont(boldItalicFontPath, fontSize_0_67X, m_UsePtSize);
				
				FontManager::GetInstance()->MarkForCleanup();

				PrepareMarkdownText(characterFaces);
			}
			else {
				m_Fonts.clear();

				std::string defaultFontPath = m_FontPath + ".ttf";

				m_Fonts[SIZE_1_00X] = FontManager::GetInstance()->GetFont(defaultFontPath, m_FontSize, m_UsePtSize);

				FontManager::GetInstance()->MarkForCleanup();

				PrepareText(m_Text, m_LineSpacing, m_Fonts[SIZE_1_00X].get(), characterFaces, m_MaxWidth, glm::vec2(0.0f, 0.0f), m_Min, m_Max, m_Color);
			}
			//m_TranslationOffset = glm::round(m_Max - ((glm::abs(m_Min) + glm::abs(m_Max)) * 0.5f));
			m_TranslationOffset = glm::vec2(0.0f, glm::round(m_Max.y - ((glm::abs(m_Min.y) + glm::abs(m_Max.y)) * 0.5f)));

			glGenVertexArrays(1, &m_VAO);
			glBindVertexArray(m_VAO);

			glGenBuffers(1, &m_VBO);
			glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
			glBufferData(GL_ARRAY_BUFFER, stringLength * sizeof(GUI::Character::Face), characterFaces, GL_DYNAMIC_DRAW);

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
		m_MaxWidth = size;
	}

	void Text::SetUsePtSize(bool usePtSize)
	{
		m_ShouldRecalculate = (m_UsePtSize != usePtSize) || m_ShouldRecalculate;
		m_UsePtSize = usePtSize;
	}

	void Text::SetSupportsMarkdown(bool supportsMarkdown)
	{
		m_ShouldRecalculate = (m_SupportsMarkdown != supportsMarkdown) || m_ShouldRecalculate;
		m_SupportsMarkdown = supportsMarkdown;
	}

	void Text::SetAlignment(Alignment alignment)
	{
		Alignment tempAlignment = GUI::Text::Alignment::LEFT;
		switch (alignment)
		{
		case GUI::Text::Alignment::DEFAULT:
			tempAlignment = Alignment::LEFT;
			break;
		case GUI::Text::Alignment::PREVIOUS:
			tempAlignment = m_Alignment;
			break;
		case GUI::Text::Alignment::LEFT:
			tempAlignment = Alignment::LEFT;
			break;
		case GUI::Text::Alignment::CENTER:
			tempAlignment = Alignment::CENTER;
			break;
		case GUI::Text::Alignment::RIGHT:
			tempAlignment = Alignment::RIGHT;
			break;
		default:
			break;
		}
		m_ShouldRecalculate = (m_Alignment != tempAlignment) || m_ShouldRecalculate;
		m_Alignment = tempAlignment;
	}

	void Text::SetNumberOfRows(unsigned int numRows)
	{
		m_ShouldRecalculate = (m_NumberOfRows != numRows) || m_ShouldRecalculate;
		m_NumberOfRows = numRows;
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

	bool Text::GetUsePtSize()
	{
		return m_UsePtSize;
	}

	bool Text::GetSupportsMarkdown()
	{
		return m_SupportsMarkdown;
	}

	Text::Alignment Text::GetAlignment()
	{
		return m_Alignment;
	}

	unsigned int Text::GetNumberOfRows()
	{
		return m_NumberOfRows;
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