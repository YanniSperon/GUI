#pragma once

#include "View.h"
#include "Texture.h"

#include <string>

namespace GUI {
	class ImageView : public View {
	private:

	protected:
		Texture* m_Texture;
	public:
		ImageView(View* parent);
		virtual ~ImageView() override;

		void SetTexture(const std::string& name);

		virtual void Bind(const glm::mat4& projection, Shader& shader) override;
	};
}