#include "TextView.h"

namespace GUI {
	TextView::TextView(View* parent)
		: View(parent), m_Text()
	{

	}

	TextView::~TextView()
	{

	}

	Text& TextView::GetText()
	{
		return m_Text;
	}

	void TextView::Recalculate()
	{
		View::Recalculate();
		m_Text.SetTranslation(glm::vec2(m_MaximumBounds - m_MinimumBounds));
		m_Text.SetMaxWidth(m_MaximumBounds.x - m_MinimumBounds.x);
	}

	void TextView::Draw(const glm::mat4& projection)
	{
		View::Draw(projection);
		m_Text.Draw(projection);
	}
}