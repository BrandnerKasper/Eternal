#pragma once

#include "Eternal/Scene/ScriptableEntity.h"
#include "Player.h"
#include "Rotater.h"
#include "Deathbox.h"
#include "MoveableXPlatform.h"
#include "SceneCameraController.h"
#include "MoveableDeathbox.h"
#include "MoveableYPlatform.h"
#include "JumpWallR.h"
#include "JumpWallL.h"
#include "BackgroundMusic.h"
#include "PressStart.h"

namespace Eternal {

	enum class Scripts
	{
		EPlayer = 0,
		ERotater = 1,
		EDeathbox = 2,
		EMoveableXPlatform = 3,
		ESceneCameraController = 4,
		EMoveableDeathbox = 5,
		EMoveableYPlatform = 6,
		EJumpWallR = 7,
		EJumpWallL = 8,
		EBackgroundMusic = 9,
		EPressStart = 10
	};

	class ScriptHandler 
	{
	public:

		static ScriptableEntity* GetScript(Scripts script)
		{
			switch (script)
			{
			case Scripts::EPlayer:
				return new Player();
				break;
			case Scripts::ERotater:
				return new Rotater();
				break;
			case Scripts::EDeathbox:
				return new Deathbox();
				break;
			case Scripts::EMoveableXPlatform:
				return new MoveableXPlatform();
				break;
			case Scripts::ESceneCameraController:
				return new SceneCameraController();
				break;
			case Scripts::EMoveableDeathbox:
				return new MoveableDeathbox();
				break;
			case Scripts::EMoveableYPlatform:
				return new MoveableYPlatform();
				break;
			case Scripts::EJumpWallR:
				return new JumpWallR();
				break;
			case Scripts::EJumpWallL:
				return new JumpWallL();
				break;
			case Scripts::EBackgroundMusic:
				return new BackgroundMusic();
				break;
			case Scripts::EPressStart:
				return new PressStart();
				break;
			default:
				return nullptr;
			}
		}

		static Scripts GetScriptNumber(std::string scriptname)
		{
			if (scriptname._Equal("Player.h"))
				return Scripts::EPlayer;
			if (scriptname._Equal("Rotater.h"))
				return Scripts::ERotater;
			if (scriptname._Equal("Deathbox.h"))
				return Scripts::EDeathbox;
			if (scriptname._Equal("MoveableXPlatform.h"))
				return Scripts::EMoveableXPlatform;
			if (scriptname._Equal("SceneCameraController.h"))
				return Scripts::ESceneCameraController;
			if (scriptname._Equal("MoveableDeathbox.h"))
				return Scripts::EMoveableDeathbox;
			if (scriptname._Equal("MoveableYPlatform.h"))
				return Scripts::EMoveableYPlatform;
			if (scriptname._Equal("JumpWallR.h"))
				return Scripts::EJumpWallR;
			if (scriptname._Equal("JumpWallL.h"))
				return Scripts::EJumpWallL;
			if (scriptname._Equal("BackgroundMusic.h"))
				return Scripts::EBackgroundMusic;
			if (scriptname._Equal("PressStart.h"))
				return Scripts::EPressStart;
			ET_CORE_ASSERT(false, "Script not initialized!");
		}
	};
}