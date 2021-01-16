#pragma once
#include "View.h"
#include "Text.h"

#include <memory>
#include <string>

namespace GUI {
	class TextView : public View {
	private:
		Text m_Text;
	public:
		TextView(View* parent);
		virtual ~TextView() override;

		Text& GetText();

		virtual void Recalculate() override;

		virtual void Draw(const glm::mat4& projection) override;
	};
}