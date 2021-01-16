#pragma once

#include <glm.hpp>
#include <functional>

namespace GUI {
	enum class State {
		POSSIBLE = 0,
		BEGAN = 1,
		CHANGED = 2,
		ENDED = 3,
	};

	class Gesture {
	protected:
		glm::vec2 m_MinimumBounds;
		glm::vec2 m_MaximumBounds;

		std::function<void()> m_CallbackFunction;

	public:
		Gesture(const glm::vec2& min, const glm::vec2& max);
		~Gesture();

		//void TouchesBegan(_ touches : Set<UITouch>, with event : UIEvent) {
		//	
		//}
		//
		//override func TouchesMoved(_ touches : Set<UITouch>, with event : UIEvent) {
		//	
		//}
		//
		//override func TouchesEnded(_ touches : Set<UITouch>, with event : UIEvent) {
		//	reset()
		//}
		//
		//override func TouchesCancelled(_ touches : Set<UITouch>, with event : UIEvent) {
		//	reset()
		//}

		void SetBounds(const glm::vec2& min, const glm::vec2& max);

		void Update(float deltaTime);
	};
}