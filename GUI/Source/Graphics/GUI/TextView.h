/*#pragma once
#include "View.h"
#include "Text.h"

#include <memory>

namespace GUI {
	class TextView : public View {
	private:
		std::unique_ptr<Text> m_Text;
	public:
		TextView(View* parent);
		virtual ~TextView() override;

		void SetText();

		virtual void Bind(const glm::mat4& projection, Shader& shader) override;
	};
}*/