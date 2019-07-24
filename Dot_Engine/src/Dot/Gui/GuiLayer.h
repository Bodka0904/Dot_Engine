#pragma once
#include "Dot/Layer.h"
#include "Dot/Terrain/TerrainEditor.h"


namespace Dot {

	class GuiLayer : public Layer
	{
	public:
		GuiLayer();
		~GuiLayer();

		void OnAttach() override;
		void OnUpdate(Timestep ts) override;

		void ButtonAction();
		void CheckBoxAction();
		void ArrowButtonAction();

		void TestVisibleOff();
		void TestVisibleOn();

	private:
		// TEST PURPOSE
		TerrainEditor editor;
		std::shared_ptr<Terrain> terrain;


		struct Wrappers
		{
			int main_wrapper;
			int test_wrapper;
		};

		Wrappers wrap;
	};
}
