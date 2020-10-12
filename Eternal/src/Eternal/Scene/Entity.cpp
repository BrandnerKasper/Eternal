#include "ETpch.h"
#include "Entity.h"

namespace Eternal {

	Entity::Entity(entt::entity handle, Scene* scene)
		: m_EntityHandle(handle), m_Scene(scene)
	{
	}
}