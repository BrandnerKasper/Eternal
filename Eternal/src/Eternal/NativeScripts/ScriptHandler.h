#pragma once

#include "Eternal/Scene/ScriptableEntity.h"
#include "Controller.h"
#include "Rotater.h"

namespace Eternal {

	enum class Scripts
	{
		EController = 0,
		ERotater = 1
	};

	class ScriptHandler 
	{
	public:

		static ScriptableEntity* GetScript(Scripts script)
		{
			switch (script)
			{
			case Scripts::EController:
				return new Controller();
				break;
			case Scripts::ERotater:
				return new Rotater();
				break;
			default:
				return nullptr;
			}
		}

		static Scripts GetScriptNumber(std::string scriptname)
		{
			if (scriptname._Equal("Controller.h"))
				return Scripts::EController;
			if (scriptname._Equal("Rotater.h"))
				return Scripts::ERotater;
			ET_CORE_ASSERT(false, "Script not initialized!");
		}
	};
}