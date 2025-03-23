#pragma once

#include "Shader.h"

#include <glm.hpp>
#include <glew.h>
#include <vector>
#include <memory>

namespace GUI {
	class View {
	public:
		enum class SizeMode {
			FILL = 1,
			ASPECT_FIT = 2,
			ASPECT_FILL = 4
		};
		enum class ConstraintMeasurementType {
			PERCENT = 1,
			PIXELS = 2
		};
		enum class YConstraintLocation {
			TOP = 1,
			CENTER = 2,
			BOTTOM = 4
		};
		enum class XConstraintLocation {
			LEFT = 1,
			CENTER = 2,
			RIGHT = 4
		};
		enum class HitBehavior {
			ABSORB = 1,
			PASSTHROUGH = 2
		};
	private:
		float m_WidthConstraint;
		ConstraintMeasurementType m_WidthConstraintMeasurementType;
		
		float m_HeightConstraint;
		ConstraintMeasurementType m_HeightConstraintMeasurementType;

		float m_XConstraintOffset;
		XConstraintLocation m_XConstraintLocation;
		ConstraintMeasurementType m_XConstraintOffsetMeasurementType;

		float m_YConstraintOffset;
		YConstraintLocation m_YConstraintLocation;
		ConstraintMeasurementType m_YConstraintOffsetMeasurementType;

		GLuint m_VAO;
		GLuint m_VBO;
	protected:
		View* m_Parent;
		std::vector<View*> m_Children;

		std::shared_ptr<Shader> m_Shader;

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
		SizeMode m_SizeMode;

		HitBehavior m_HitBehavior;

		virtual void SetBounds(const glm::vec2& min, const glm::vec2& max);

	public:
		View(float x, float y, float width, float height);
		View(View* parent);
		virtual ~View();

		void SetWidthConstraint(float width, ConstraintMeasurementType type);
		void SetHeightConstraint(float height, ConstraintMeasurementType type);
		void SetXConstraint(float offset, ConstraintMeasurementType measurementType, XConstraintLocation location);
		void SetYConstraint(float offset, ConstraintMeasurementType measurementType, YConstraintLocation location);
		void SetSizeMode(SizeMode sizeMode, float aspectRatio);

		virtual View* HitTest(double x, double y);



		glm::mat4 GetTransformationMatrix();
		glm::mat4 GetTranslationMatrix();
		glm::mat4 GetScaleMatrix();

		void Recalculate(float x, float y, float width, float height);
		virtual void Recalculate();

		virtual void Bind(const glm::mat4& projection);
		virtual void Draw(const glm::mat4& projection);

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



		void SetHitBehavior(HitBehavior hb);
		HitBehavior GetHitBehavior();
	};
}