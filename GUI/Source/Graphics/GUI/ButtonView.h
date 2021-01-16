#pragma once
#include "View.h"

#include <functional>

namespace GUI {
	class ButtonView: public View {
	protected:
		std::function<void()> m_CallbackFunction;
	public:
		ButtonView(View* parent);
		virtual ~ButtonView() override;

		virtual View* HitTest(double x, double y) override;

		void SetCallbackFunction(const std::function<void()>& callback);
	};
}