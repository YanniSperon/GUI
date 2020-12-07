#include "View.h"
#include "Console.h"
#include "Texture.h"

#include <glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace GUI {
	View::View(float x, float y, float width, float height)
		: m_WidthConstraint(50.0f), m_WidthConstraintMeasurementType(VIEW_CONSTRAINT_MEASUREMENT_TYPE_PERCENT), m_HeightConstraint(50.0f), m_HeightConstraintMeasurementType(VIEW_CONSTRAINT_MEASUREMENT_TYPE_PERCENT), m_XConstraintOffset(0.0f), m_XConstraintLocation(VIEW_CONSTRAINT_LOCATION_CENTER), m_XConstraintOffsetMeasurementType(VIEW_CONSTRAINT_MEASUREMENT_TYPE_PERCENT), m_YConstraintOffset(0.0f), m_YConstraintLocation(VIEW_CONSTRAINT_LOCATION_CENTER), m_YConstraintOffsetMeasurementType(VIEW_CONSTRAINT_MEASUREMENT_TYPE_PERCENT), m_VAO(0), m_VBO(0), m_Scale(1.0f), m_Translation(0.0f), m_MinimumWidth(1.0f), m_MinimumHeight(1.0f), m_Opacity(1.0f), m_Children(), m_Parent(nullptr), m_CornerRoundness(0.0625f), m_TintColor(1.0f, 1.0f, 1.0f, 1.0f), m_MinimumBounds(x, y), m_MaximumBounds(x + width, y + height), m_AspectRatio(1.0f), m_ShouldClipToBounds(false), m_SizeMode(VIEW_SIZE_MODE_FILL), m_BorderWeight(0.0f), m_BorderColor(0.0f, 0.0f, 0.0f, 1.0f)
	{
		GLfloat vertices[] = {
			-0.5f, -0.5f, // Bottom Left
			0.0f, 0.0f,   // 
			-0.5f, 0.5f,  // Top Left
			0.0f, 1.0f,   // 
			0.5f, 0.5f,   // Top Right
			1.0f, 1.0f,   // 

			-0.5f, -0.5f, // Bottom Left
			0.0f, 0.0f,   // 
			0.5f, 0.5f,   // Top Right
			1.0f, 1.0f,   // 
			0.5f, -0.5f,  // Bottom Right
			1.0f, 0.0f    // 
		};

		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);

		glGenBuffers(1, &m_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(sizeof(GLfloat) * 2));

		Recalculate(x, y, width, height);
	}

	View::View(View* parent)
		: m_WidthConstraint(50.0f), m_WidthConstraintMeasurementType(VIEW_CONSTRAINT_MEASUREMENT_TYPE_PERCENT),  m_HeightConstraint(50.0f), m_HeightConstraintMeasurementType(VIEW_CONSTRAINT_MEASUREMENT_TYPE_PERCENT), m_XConstraintOffset(0.0f), m_XConstraintLocation(VIEW_CONSTRAINT_LOCATION_CENTER), m_XConstraintOffsetMeasurementType(VIEW_CONSTRAINT_MEASUREMENT_TYPE_PERCENT), m_YConstraintOffset(0.0f), m_YConstraintLocation(VIEW_CONSTRAINT_LOCATION_CENTER), m_YConstraintOffsetMeasurementType(VIEW_CONSTRAINT_MEASUREMENT_TYPE_PERCENT), m_VAO(0), m_VBO(0), m_Scale(1.0f), m_Translation(0.0f), m_MinimumWidth(1.0f), m_MinimumHeight(1.0f), m_Opacity(1.0f), m_Children(), m_Parent(parent), m_CornerRoundness(0.0625f), m_TintColor(1.0f, 1.0f, 1.0f, 1.0f), m_MinimumBounds(parent->m_MinimumBounds), m_MaximumBounds(parent->m_MaximumBounds), m_SizeMode(VIEW_SIZE_MODE_FILL), m_AspectRatio(1.0f), m_ShouldClipToBounds(false), m_BorderWeight(0.0f), m_BorderColor(0.0f, 0.0f, 0.0f, 1.0f)
	{
		m_Parent->AddChild(this);
		GLfloat vertices[] = {
			-0.5f, -0.5f, // Bottom Left
			0.0f, 0.0f,   // 
			-0.5f, 0.5f,  // Top Left
			0.0f, 1.0f,   // 
			0.5f, 0.5f,   // Top Right
			1.0f, 1.0f,   // 

			-0.5f, -0.5f, // Bottom Left
			0.0f, 0.0f,   // 
			0.5f, 0.5f,   // Top Right
			1.0f, 1.0f,   // 
			0.5f, -0.5f,  // Bottom Right
			1.0f, 0.0f    // 
		};

		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);

		glGenBuffers(1, &m_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(sizeof(GLfloat) * 2));

		Recalculate();
	}

	View::~View()
	{
		glDeleteBuffers(1, &m_VBO);
		glDeleteVertexArrays(1, &m_VAO);
	}

	void View::Recalculate(float x, float y, float width, float height)
	{
		float parentXMin = x;
		float parentXMax = x + width;
		float parentYMin = y;
		float parentYMax = y + height;



		// Width
		int finalWidthPixels = 1;
		switch (m_WidthConstraintMeasurementType)
		{
		case VIEW_CONSTRAINT_MEASUREMENT_TYPE_PERCENT:
			finalWidthPixels = glm::round((parentXMax - parentXMin) * (m_WidthConstraint / 100.0f));
			break;
		case VIEW_CONSTRAINT_MEASUREMENT_TYPE_PIXELS:
			finalWidthPixels = glm::round(m_WidthConstraint);
			break;
		default:
			break;
		}
		if (finalWidthPixels < m_MinimumWidth) {
			finalWidthPixels = glm::round(m_MinimumWidth);
		}



		// Height
		int finalHeightPixels = 1;
		switch (m_HeightConstraintMeasurementType)
		{
		case VIEW_CONSTRAINT_MEASUREMENT_TYPE_PERCENT:
			finalHeightPixels = glm::round((parentYMax - parentYMin) * (m_HeightConstraint / 100.0f));
			break;
		case VIEW_CONSTRAINT_MEASUREMENT_TYPE_PIXELS:
			finalHeightPixels = glm::round(m_HeightConstraint);
			break;
		default:
			break;
		}
		if (finalHeightPixels < m_MinimumHeight) {
			finalHeightPixels = glm::round(m_MinimumHeight);
		}



		// X
		int xPixelOffset = 0;
		switch (m_XConstraintOffsetMeasurementType)
		{
		case VIEW_CONSTRAINT_MEASUREMENT_TYPE_PERCENT:
			xPixelOffset = glm::round((parentXMax - parentXMin) * (m_XConstraintOffset / 100.0f));
			break;
		case VIEW_CONSTRAINT_MEASUREMENT_TYPE_PIXELS:
			xPixelOffset = glm::round(m_XConstraintOffset);
			break;
		default:
			break;
		}
		int xPixelDefaultSpot = 0;
		switch (m_XConstraintLocation)
		{
		case VIEW_CONSTRAINT_LOCATION_LEFT:
			xPixelDefaultSpot = glm::round(parentXMin + (finalWidthPixels / 2.0f));
			break;
		case VIEW_CONSTRAINT_LOCATION_CENTER:
			xPixelDefaultSpot = glm::round(parentXMin + ((parentXMax - parentXMin) / 2.0f));
			break;
		case VIEW_CONSTRAINT_LOCATION_RIGHT:
			xPixelDefaultSpot = glm::round(parentXMax - (finalWidthPixels / 2.0f));
			break;
		default:
			break;
		}
		int finalXTranslation = xPixelDefaultSpot + xPixelOffset;



		// Y
		int yPixelOffset = 0;
		switch (m_XConstraintOffsetMeasurementType)
		{
		case VIEW_CONSTRAINT_MEASUREMENT_TYPE_PERCENT:
			yPixelOffset = glm::round((parentYMax - parentYMin) * (m_YConstraintOffset / 100.0f));
			break;
		case VIEW_CONSTRAINT_MEASUREMENT_TYPE_PIXELS:
			yPixelOffset = glm::round(m_YConstraintOffset);
			break;
		default:
			break;
		}
		int yPixelDefaultSpot = 0;
		switch (m_YConstraintLocation)
		{
		case VIEW_CONSTRAINT_LOCATION_TOP:
			yPixelDefaultSpot = glm::round(parentYMax - (finalHeightPixels / 2.0f));
			break;
		case VIEW_CONSTRAINT_LOCATION_CENTER:
			yPixelDefaultSpot = glm::round(parentYMin + ((parentYMax - parentYMin) / 2.0f));
			break;
		case VIEW_CONSTRAINT_LOCATION_BOTTOM:
			yPixelDefaultSpot = glm::round(parentYMin + (finalHeightPixels / 2.0f));
			break;
		default:
			break;
		}
		int finalYTranslation = yPixelDefaultSpot + yPixelOffset;



		m_Translation = glm::vec2(finalXTranslation, finalYTranslation);
		m_Scale = glm::vec2(finalWidthPixels, finalHeightPixels);
		m_MinimumBounds = glm::vec2(m_Translation - (m_Scale/2.0f));
		m_MaximumBounds = glm::vec2(m_Translation + (m_Scale / 2.0f));

		for (int i = 0; i < m_Children.size(); i++) {
			m_Children[i]->Recalculate();
		}
	}

	void View::Recalculate()
	{
		float parentXMin = m_Parent->m_MinimumBounds.x;
		float parentXMax = m_Parent->m_MaximumBounds.x;
		float parentYMin = m_Parent->m_MinimumBounds.y;
		float parentYMax = m_Parent->m_MaximumBounds.y;



		int finalWidthPixels = 1 ;
		switch (m_WidthConstraintMeasurementType)
		{
		case VIEW_CONSTRAINT_MEASUREMENT_TYPE_PERCENT:
			finalWidthPixels = glm::round((parentXMax - parentXMin) * (m_WidthConstraint / 100.0f));
			break;
		case VIEW_CONSTRAINT_MEASUREMENT_TYPE_PIXELS:
			finalWidthPixels = glm::round(m_WidthConstraint);
			break;
		default:
			break;
		}
		if (finalWidthPixels < m_MinimumWidth) {
			finalWidthPixels = glm::round(m_MinimumWidth);
		}
		// Height
		int finalHeightPixels = 1;
		switch (m_HeightConstraintMeasurementType)
		{
		case VIEW_CONSTRAINT_MEASUREMENT_TYPE_PERCENT:
			finalHeightPixels = glm::round((parentYMax - parentYMin) * (m_HeightConstraint / 100.0f));
			break;
		case VIEW_CONSTRAINT_MEASUREMENT_TYPE_PIXELS:
			finalHeightPixels = glm::round(m_HeightConstraint);
			break;
		default:
			break;
		}
		if (finalHeightPixels < m_MinimumHeight) {
			finalHeightPixels = glm::round(m_MinimumHeight);
		}

		

		switch (m_SizeMode)
		{
		case VIEW_SIZE_MODE_ASPECT_FILL:
			if (finalHeightPixels >= finalWidthPixels) {
				finalWidthPixels = glm::round(m_AspectRatio * ((float)finalHeightPixels));
			}
			else if (finalHeightPixels < finalWidthPixels) {
				finalHeightPixels = glm::round((1.0f / m_AspectRatio) * ((float)finalWidthPixels));
			}
			break;
		case VIEW_SIZE_MODE_ASPECT_FIT:
			if (finalWidthPixels >= finalHeightPixels) {
				finalWidthPixels = glm::round(m_AspectRatio * ((float)finalHeightPixels));
			}
			else if (finalWidthPixels < finalHeightPixels) {
				finalHeightPixels = glm::round((1.0f / m_AspectRatio) * ((float)finalWidthPixels));
			}
			break;
		case VIEW_SIZE_MODE_FILL:
			// Leave as is
			break;
		default:
			break;
		}



		// X
		int xPixelOffset = 0;
		switch (m_XConstraintOffsetMeasurementType)
		{
		case VIEW_CONSTRAINT_MEASUREMENT_TYPE_PERCENT:
			xPixelOffset = glm::round((parentXMax - parentXMin) * (m_XConstraintOffset / 100.0f));
			break;
		case VIEW_CONSTRAINT_MEASUREMENT_TYPE_PIXELS:
			xPixelOffset = glm::round(m_XConstraintOffset);
			break;
		default:
			break;
		}
		int xPixelDefaultSpot = 0;
		switch (m_XConstraintLocation)
		{
		case VIEW_CONSTRAINT_LOCATION_LEFT:
			xPixelDefaultSpot = glm::round(parentXMin + (finalWidthPixels / 2.0f));
			break;
		case VIEW_CONSTRAINT_LOCATION_CENTER:
			xPixelDefaultSpot = glm::round(parentXMin + ((parentXMax - parentXMin)/2.0f));
			break;
		case VIEW_CONSTRAINT_LOCATION_RIGHT:
			xPixelDefaultSpot = glm::round(parentXMax - (finalWidthPixels / 2.0f));
			break;
		default:
			break;
		}
		int finalXTranslation = xPixelDefaultSpot + xPixelOffset;



		// Y
		int yPixelOffset = 0;
		switch (m_XConstraintOffsetMeasurementType)
		{
		case VIEW_CONSTRAINT_MEASUREMENT_TYPE_PERCENT:
			yPixelOffset = glm::round((parentYMax - parentYMin) * (m_YConstraintOffset / 100.0f));
			break;
		case VIEW_CONSTRAINT_MEASUREMENT_TYPE_PIXELS:
			yPixelOffset = glm::round(m_YConstraintOffset);
			break;
		default:
			break;
		}
		int yPixelDefaultSpot = 0;
		switch (m_YConstraintLocation)
		{
		case VIEW_CONSTRAINT_LOCATION_TOP:
			yPixelDefaultSpot = glm::round(parentYMax - (finalHeightPixels / 2.0f));
			break;
		case VIEW_CONSTRAINT_LOCATION_CENTER:
			yPixelDefaultSpot = glm::round(parentYMin + ((parentYMax - parentYMin) / 2.0f));
			break;
		case VIEW_CONSTRAINT_LOCATION_BOTTOM:
			yPixelDefaultSpot = glm::round(parentYMin + (finalHeightPixels / 2.0f));
			break;
		default:
			break;
		}
		int finalYTranslation = yPixelDefaultSpot + yPixelOffset;



		m_Translation = glm::vec2(finalXTranslation, finalYTranslation);
		m_Scale = glm::vec2(finalWidthPixels, finalHeightPixels);
		m_MinimumBounds = glm::vec2(m_Translation - (m_Scale / 2.0f));
		m_MaximumBounds = glm::vec2(m_Translation + (m_Scale / 2.0f));

		for (int i = 0; i < m_Children.size(); i++) {
			m_Children[i]->Recalculate();
		}
	}

	void View::SetWidthConstraint(float width, int type)
	{
		m_WidthConstraint = width;
		m_WidthConstraintMeasurementType = type;
	}

	void View::SetHeightConstraint(float height, int type)
	{
		m_HeightConstraint = height;
		m_HeightConstraintMeasurementType = type;
	}

	void View::SetXConstraint(float offset, int offsetType, int location)
	{
		m_XConstraintOffset = offset;
		m_XConstraintOffsetMeasurementType = offsetType;
		m_XConstraintLocation = location;
	}

	void View::SetYConstraint(float offset, int offsetType, int location)
	{
		m_YConstraintOffset = offset;
		m_YConstraintOffsetMeasurementType = offsetType;
		m_YConstraintLocation = location;
	}

	void View::SetSizeMode(int sizeMode, float aspectRatio)
	{
		m_SizeMode = sizeMode;
		m_AspectRatio = aspectRatio;
	}

	View* View::HitTest(double x, double y)
	{
		View* temp = nullptr;
		for (int i = 0; i < m_Children.size(); i++) {
			if (!temp) {
				temp = m_Children[i]->HitTest(x, y);
			}
			else {
				break;
			}
		}
		if (!temp && x > m_MinimumBounds.x && x < m_MaximumBounds.x &&
			y > m_MinimumBounds.y && y < m_MaximumBounds.y) {
			return this;
		}
		return temp;
	}

	glm::mat4 View::GetTransformationMatrix()
	{
		return glm::translate(glm::mat4(1.0f), glm::vec3(m_Translation, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(m_Scale, 1.0f));
	}

	glm::mat4 View::GetTranslationMatrix()
	{
		return glm::translate(glm::mat4(1.0f), glm::vec3(m_Translation, 0.0f));
	}

	glm::mat4 View::GetScaleMatrix()
	{
		return glm::scale(glm::mat4(1.0f), glm::vec3(m_Scale, 1.0f));
	}

	void View::Bind(const glm::mat4& projection, Shader& shader)
	{
		shader.Bind();
		shader.SetMat4("u_P", projection);
		shader.SetMat4("u_M", GetTransformationMatrix());
		shader.SetVec4("u_TintColor", m_TintColor);
		shader.SetFloat("u_Opacity", m_Opacity);
		shader.SetFloat("u_CornerRoundness", m_CornerRoundness);
		shader.SetInt("u_ShouldClipToBounds", m_Parent && m_ShouldClipToBounds);
		if (m_Parent) {
			shader.SetFloat("u_ParentCornerRoundness", m_Parent->m_CornerRoundness);
			shader.SetVec2("u_ParentScale", m_Parent->m_Scale);
			shader.SetVec2("u_ParentTranslation", m_Parent->m_Translation);
		}
		shader.SetInt("u_HasImageBackground", 0);
		shader.SetFloat("u_BorderWeight", m_BorderWeight);
		shader.SetVec4("u_BorderColor", m_BorderColor);
		Texture::Unbind(0);
	}

	void View::Draw(const glm::mat4& projection, Shader& shader)
	{
		Bind(projection, shader);

		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		for (int i = 0; i < m_Children.size(); i++) {
			m_Children[i]->Draw(projection, shader);
		}
	}

	void View::AddChild(View* view)
	{
		m_Children.push_back(view);
		view->m_Parent = this;
	}

	void View::SetParent(View* parent)
	{
		parent->AddChild(this);
		m_Parent = parent;
	}

	void View::SetOpacity(float opacity)
	{
		m_Opacity = opacity;
	}

	float View::GetOpacity()
	{
		return m_Opacity;
	}

	void View::SetCornerRoundness(float roundness)
	{
		m_CornerRoundness = roundness;
	}

	float View::GetCornerRoundness()
	{
		return m_CornerRoundness;
	}

	void View::SetBorderWeight(float borderWeight)
	{
		m_BorderWeight = borderWeight;
	}

	float View::GetBorderWeight()
	{
		return m_BorderWeight;
	}
	
	void View::SetTintColor(const glm::vec4& tintColor)
	{
		m_TintColor = tintColor;
	}

	const glm::vec4& View::GetTintColor()
	{
		return m_TintColor;
	}

	void View::SetBorderColor(const glm::vec4& borderColor)
	{
		m_BorderColor = borderColor;
	}

	const glm::vec4& View::GetBorderColor()
	{
		return m_BorderColor;
	}

	void View::SetShouldClipToBounds(bool shouldClipToBounds)
	{
		m_ShouldClipToBounds = shouldClipToBounds;
	}

	bool View::GetShouldClipToBounds()
	{
		return m_ShouldClipToBounds;
	}

	float View::GetAspectRatio()
	{
		return m_AspectRatio;
	}

	void View::SetMinimumWidth(float minimumWidth)
	{
		m_MinimumWidth = minimumWidth;
	}

	float View::GetMinimumWidth()
	{
		return m_MinimumWidth;
	}

	void View::SetMinimumHeight(float minimumHeight)
	{
		m_MinimumHeight = minimumHeight;
	}
	float View::GetMinimumHeight()
	{
		return m_MinimumHeight;
	}
}