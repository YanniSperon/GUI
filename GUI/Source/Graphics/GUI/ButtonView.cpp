#include "ButtonView.h"
#include "Console.h"

namespace GUI {
	ButtonView::ButtonView(View* parent)
		: View(parent)
	{

	}

	ButtonView::~ButtonView()
	{

	}

	View* ButtonView::HitTest(double x, double y)
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
			m_CallbackFunction();
			return this;
		}
		return temp;
	}

	void ButtonView::SetCallbackFunction(const std::function<void()>& callback)
	{
		m_CallbackFunction = callback;
	}
}