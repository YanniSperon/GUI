#pragma once

#include "Shader.h"

#include <glm.hpp>
#include <glew.h>
#include <vector>

#define VIEW_CONSTRAINT_MEASUREMENT_TYPE_PERCENT 1
#define VIEW_CONSTRAINT_MEASUREMENT_TYPE_PIXELS 2

#define VIEW_CONSTRAINT_LOCATION_LEFT 1
#define VIEW_CONSTRAINT_LOCATION_TOP 1
#define VIEW_CONSTRAINT_LOCATION_CENTER 2
#define VIEW_CONSTRAINT_LOCATION_RIGHT 4
#define VIEW_CONSTRAINT_LOCATION_BOTTOM 4

#define VIEW_SIZE_MODE_FILL 1
#define VIEW_SIZE_MODE_ASPECT_FIT 2
#define VIEW_SIZE_MODE_ASPECT_FILL 4

namespace GUI {
	class View {
	private:
		float m_WidthConstraint;
		int m_WidthConstraintMeasurementType;
		
		float m_HeightConstraint;
		int m_HeightConstraintMeasurementType;

		float m_XConstraintOffset;
		int m_XConstraintLocation;
		int m_XConstraintOffsetMeasurementType;

		float m_YConstraintOffset;
		int m_YConstraintLocation;
		int m_YConstraintOffsetMeasurementType;

		GLuint m_VAO;
		GLuint m_VBO;
	protected:
		View* m_Parent;
		std::vector<View*> m_Children;

		glm::vec4 m_TintColor;
		float m_Opacity;
		float m_CornerRoundness;
		float m_BorderWeight;
		glm::vec4 m_BorderColor;

		glm::vec2 m_Translation;
		glm::vec2 m_Scale;
		glm::vec2 m_MinimumBounds;
		glm::vec2 m_MaximumBounds;

		float m_MinimumWidth;
		float m_MinimumHeight;

		float m_AspectRatio;

		bool m_ShouldClipToBounds;
		int m_SizeMode;

	public:
		View(float x, float y, float width, float height);
		View(View* parent);
		virtual ~View();

		void SetWidthConstraint(float width, int type);
		void SetHeightConstraint(float height, int type);
		void SetXConstraint(float offset, int offsetType, int location);
		void SetYConstraint(float offset, int offsetType, int location);
		void SetSizeMode(int sizeMode, float aspectRatio);

		View* HitTest(double x, double y);



		glm::mat4 GetTransformationMatrix();
		glm::mat4 GetTranslationMatrix();
		glm::mat4 GetScaleMatrix();

		void Recalculate(float x, float y, float width, float height);
		void Recalculate();

		virtual void Bind(const glm::mat4& projection, Shader& shader);
		virtual void Draw(const glm::mat4& projection, Shader& shader);

		void AddChild(View* view);
		void SetParent(View* parent);



		void SetOpacity(float opacity);
		float GetOpacity();

		void SetCornerRoundness(float roundness);
		float GetCornerRoundness();

		void SetBorderWeight(float borderWeight);
		float GetBorderWeight();

		void SetTintColor(const glm::vec4& tintColor);
		const glm::vec4& GetTintColor();

		void SetBorderColor(const glm::vec4& borderColor);
		const glm::vec4& GetBorderColor();

		void SetShouldClipToBounds(bool shouldClipToBounds);
		bool GetShouldClipToBounds();

		float GetAspectRatio();



		void SetMinimumWidth(float minimumWidth);
		float GetMinimumWidth();
		void SetMinimumHeight(float minimumHeight);
		float GetMinimumHeight();
	};
}