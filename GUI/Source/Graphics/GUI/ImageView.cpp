#include "ImageView.h"
#include "Console.h"

namespace GUI {
	ImageView::ImageView(View* parent)
		: View(parent), m_Texture(nullptr)
	{

	}

	ImageView::~ImageView()
	{
		delete m_Texture;
	}

	void ImageView::SetTexture(const std::string& name)
	{
		if (m_Texture) {
			delete m_Texture;
		}
		m_Texture = new Texture(name);
	}

	void ImageView::Bind(const glm::mat4& projection, Shader& shader)
	{
		View::Bind(projection, shader);
		shader.SetInt("u_HasImageBackground", 1);
		if (m_Texture) {
			m_Texture->Bind(0);
		}
	}
}