#include "ETpch.h"
#include <Eternal.h>
#include <Eternal/Core/EntryPoint.h>

#include "EditorLayer.h"


namespace Eternal {

	class DoomedEditor : public Application
	{
	public:
		DoomedEditor()
			 : Application("Doomed Editor")
		{
			PushLayer(new EditorLayer());
		}
		~DoomedEditor()
		{

		}
	};

	Application* CreateApplication()
	{
		return new DoomedEditor();
	}
}