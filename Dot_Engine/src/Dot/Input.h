#pragma once

namespace Dot {
	class Input
	{
	public:
		static inline bool IsKeyPressed(int keycode) { return s_Instance->IsKeyPressedImp1(keycode); }
		static inline bool IsMouseButtonPressed(int button) { return s_Instance->IsMouseButtonPressedImp1(button); }

		static inline float GetMouseX() { return GetMouseXImp1(); }
		static inline float GetMouseY() { return GetMouseYImp1(); }


		static bool IsKeyPressedImp1(int keycode);
		static bool IsMouseButtonPressedImp1(int button);
		static std::pair<float, float>GetMousePositionImp1();

		static float GetMouseXImp1();
		static float GetMouseYImp1();


	private:
		static Input* s_Instance;

	};

}