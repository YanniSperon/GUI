#include "ImageView.h"
#include "Console.h"
#include "TextureManager.h"

namespace GUI {
	ImageView::ImageView(View* parent)
		: View(parent), m_Texture(nullptr)
	{

	}

	ImageView::~ImageView()
	{
		TextureManager::GetInstance()->MarkForCleanup();
	}

	void ImageView::SetTexture(const std::string& name)
	{
		if (m_Texture) {
			TextureManager::GetInstance()->MarkForCleanup();
		}
		m_Texture = TextureManager::GetInstance()->GetTexture(name, Texture::Type::TWOD);
	}

	void ImageView::Bind(const glm::mat4& projection)
	{
		View::Bind(projection);
		m_Shader->SetInt("u_HasImageBackground", 1);
		if (m_Texture) {
			m_Texture->Bind(0);
		}
	}
}