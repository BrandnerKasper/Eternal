#pragma once

#include "Eternal.h"
#include "Eternal/Core/Timestep.h"

namespace Eternal {

	class SettingsPanel
	{
	public:
		SettingsPanel() = default;

		void SetTimestep(Timestep ts) { m_timestep = ts; }

		void OnImGuiRender();
	private:
		SharedPtr<Scene> m_Context;
		Timestep m_timestep;
	};
}