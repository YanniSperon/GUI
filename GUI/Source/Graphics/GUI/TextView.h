#pragma once
#include "View.h"
#include "Text.h"

#include <memory>
#include <string>
/*
namespace GUI {
	class TextView : public View, public Text {
	private:
		//friend class Text;
		//Text m_Text;
	public:
		TextView(View* parent);
		virtual ~TextView() override;

		Text& GetText();

		virtual void Draw(const glm::mat4& projection, Shader& shader) override;
	protected:
		virtual void SetBounds(const glm::vec2& min, const glm::vec2& max) override;
	};
}*/