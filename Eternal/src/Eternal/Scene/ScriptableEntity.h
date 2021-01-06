#pragma once

#include "Entity.h"

namespace Eternal {

	enum class ScriptType
	{
		default,
		player,
		deathbox,
		jumpwall
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

	protected:
		Entity m_Entity;
		ScriptType m_ScriptType = ScriptType::default;
	private:
		friend class Scene;
	};
}