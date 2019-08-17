#include "Dot/Layer.h"
#include "Dot/Events/Event.h"
#include "TerrainEditor.h"


namespace Dot {
	class TerrainEditorLayer : public Layer
	{
	public:
		TerrainEditorLayer();
		~TerrainEditorLayer();

		virtual void OnAttach() override;
		virtual void OnUpdate(Timestep ts) override;
		virtual void OnEvent(Event& event) override;
		virtual void OnDetach() override;


	private:
		void NewTerrain();
		void SetHeight();
		void SetNormals();
		void RenderTerrain();
		void ChangeSize();
		void ChangeNumVertex();
		void ChangeHeight();

	private:
		std::shared_ptr<TerrainEditor> ter_editor;

		unsigned int numvertex = 150;
		float size = 300;
		float height = 0;

	};
}