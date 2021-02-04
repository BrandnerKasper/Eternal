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
#include "Load1_1.h"
#include "LoadEndScreen.h"
#include "CloseApp.h"

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
		EPressStart = 10,
		ELoad1_1 = 11,
		ELoadEndScreen = 12,
		ECloseApp = 13
	};

	class ScriptHandler 
	{
	public:

		static SharedPtr<ScriptableEntity> GetScript(Scripts script)
		{
			switch (script)
			{
			case Scripts::EPlayer:
				return CreateSharedPtr<Player>();
				break;
			case Scripts::ERotater:
				return CreateSharedPtr<Rotater>();
				break;
			case Scripts::EDeathbox:
				return CreateSharedPtr<Deathbox>();
				break;
			case Scripts::EMoveableXPlatform:
				return CreateSharedPtr<MoveableXPlatform>();
				break;
			case Scripts::ESceneCameraController:
				return CreateSharedPtr<SceneCameraController>();
				break;
			case Scripts::EMoveableDeathbox:
				return CreateSharedPtr<MoveableDeathbox>();
				break;
			case Scripts::EMoveableYPlatform:
				return CreateSharedPtr<MoveableYPlatform>();
				break;
			case Scripts::EJumpWallR:
				return CreateSharedPtr<JumpWallR>();
				break;
			case Scripts::EJumpWallL:
				return CreateSharedPtr<JumpWallL>();
				break;
			case Scripts::EBackgroundMusic:
				return CreateSharedPtr<BackgroundMusic>();
				break;
			case Scripts::EPressStart:
				return CreateSharedPtr<PressStart>();
				break;
			case Scripts::ELoad1_1:
				return CreateSharedPtr<Load1_1>();
				break;
			case Scripts::ECloseApp:
				return CreateSharedPtr<CloseApp>();
				break;
			case Scripts::ELoadEndScreen:
				return CreateSharedPtr<LoadEndScreen>();
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
			if (scriptname._Equal("Load1_1.h"))
				return Scripts::ELoad1_1;
			if (scriptname._Equal("CloseApp.h"))
				return Scripts::ECloseApp;
			if (scriptname._Equal("LoadEndScreen.h"))
				return Scripts::ELoadEndScreen;
			ET_CORE_ASSERT(false, "Script not initialized!");
		}
	};
}