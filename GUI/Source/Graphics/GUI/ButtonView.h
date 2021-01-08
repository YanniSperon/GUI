#pragma once
#include "View.h"

#include <functional>

//#define BIND_CALLBACK(x) std::bind(&x, this, std::placeholders::_1)
//int test() {
//	std::bind(&test, this);
//}

namespace GUI {
	class ButtonView: public View {
	private:
	protected:
		std::function<void()> m_CallbackFunction;
	public:
		ButtonView(View* parent);
		virtual ~ButtonView() override;

		virtual View* HitTest(double x, double y) override;

		void SetCallbackFunction(const std::function<void()>& callback);
	};
}