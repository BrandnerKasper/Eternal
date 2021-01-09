#pragma once

#include "Entity.h"
#include "Eternal/Events/KeyEvent.h"

namespace Eternal {

	enum class ScriptType
	{
		default,
		player,
		deathbox,
		jumpwallR,
		jumpwallL
	};

	class ScriptableEntity
	{
	public:
		virtual ~ScriptableEntity() {}

		template<typename T>
		T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}

		ScriptType GetScriptType() { return m_ScriptType; }

	protected:
		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnEvent(Event& event) {}
	protected:
		Entity m_Entity;
		ScriptType m_ScriptType = ScriptType::default;
	private:
		friend class Scene;
	};
}