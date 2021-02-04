#include "ETpch.h"
#include <Eternal.h>

#include "GameLayer.h"

namespace Eternal {

	class ProjectQuack : public Application
	{
	public:
		ProjectQuack()
		{
			PushLayer(CreateUniquePtr<GameLayer>());
		}

		~ProjectQuack()
		{

		}
	};

	Application* CreateApplication()
	{
		return new ProjectQuack();
	}

}