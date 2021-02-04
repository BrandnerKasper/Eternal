#include "ETpch.h"
#include "ContactListener.h"

#include "Eternal/NativeScripts/Deathbox.h"
#include "Eternal/NativeScripts/Player.h"
#include "Eternal/NativeScripts/MoveableDeathbox.h"
#include "Eternal/NativeScripts/LoadEndScreen.h"

namespace Eternal {

	/**
	* Manages the contact btw. one scriptable entity and another. For example the player entering a deathbox.
	*
	* @param ScriptableEntity* A pointer to the scritpable entity.
	* @return 
	*/
	void ManageBeginContactBtw2ScriptableEntitys(ScriptableEntity* ent_A, ScriptableEntity* ent_B)
	{
		auto scriptType_A = ent_A->GetScriptType();
		auto scriptType_B = ent_B->GetScriptType();

		switch (scriptType_A)
		{
		case ScriptType::default:
			break;
		case ScriptType::player:
			ET_CORE_INFO("A is player");
			if (scriptType_B == ScriptType::jumpwallR)
			{
				auto player = reinterpret_cast<Player*>(ent_A);
				player->HandleJumpWallRContact();
			}
			if (scriptType_B == ScriptType::jumpwallL)
			{
				auto player = reinterpret_cast<Player*>(ent_A);
				player->HandleJumpWallLContact();
			}
			break;
		case ScriptType::deathbox:
			ET_CORE_INFO("A is deathbox");
			if(scriptType_B == ScriptType::player)
			{ 
				auto deathbox = reinterpret_cast<Deathbox*>(ent_A);
				deathbox->HandlePlayerIsDead();
			}
			break;
		case ScriptType::scenetransistion:
			ET_CORE_INFO("A is scenetransistion");
			if (scriptType_B == ScriptType::player)
			{
				auto scenetransistion = reinterpret_cast<LoadEndScreen*>(ent_A);
				scenetransistion->HandlePlayerEntered();
			}
			break;
		default:
			break;
		}

		switch (scriptType_B)
		{
		case ScriptType::default:
			break;
		case ScriptType::player:
			ET_CORE_INFO("B is player");
			if (scriptType_A == ScriptType::jumpwallR)
			{
				auto player = reinterpret_cast<Player*>(ent_B);
				player->HandleJumpWallRContact();
			}
			if (scriptType_A == ScriptType::jumpwallL)
			{
				auto player = reinterpret_cast<Player*>(ent_B);
				player->HandleJumpWallLContact();
			}
			break;
		case ScriptType::deathbox:
			ET_CORE_INFO("B is deathbox");
			if(scriptType_A == ScriptType::player)
			{
				auto deathbox = reinterpret_cast<Deathbox*>(ent_B);
				deathbox->HandlePlayerIsDead();
			}
			break;
		case ScriptType::scenetransistion:
			ET_CORE_INFO("B is scenetransistion");
			if (scriptType_A == ScriptType::player)
			{
				auto scenetransistion = reinterpret_cast<LoadEndScreen*>(ent_B);
				scenetransistion->HandlePlayerEntered();
			}
			break;
		default:
			break;
		}
	}

	/**
	* Manages the contact btw. one scriptable entity and a default pyhysic entity. For example the player and a wall.
	*
	* @param ScriptableEntity* A pointer to the scritpable entity.
	* @return 
	*/
	void ManageBeginContactBtwPlayerAndDefaultPhysicsObj(ScriptableEntity* ent)
	{
		if (ent->GetScriptType() == ScriptType::player)
		{
			auto player = reinterpret_cast<Player*>(ent);
			player->HandleJumpContact();
		}
	}

	void ContactListener::BeginContact(b2Contact* contact)
	{
		ScriptableEntity* ent_A = reinterpret_cast<ScriptableEntity*>(contact->GetFixtureA()->GetBody()->GetUserData().pointer);
		ScriptableEntity* ent_B = reinterpret_cast<ScriptableEntity*>(contact->GetFixtureB()->GetBody()->GetUserData().pointer);
		
		//Handle Contact Btw. Player and Deathbox
		if (ent_A && ent_B)
			ManageBeginContactBtw2ScriptableEntitys(ent_A, ent_B);

		//Handle Contact Btw. Player and a Physics Obj.
		else if (ent_A)
			ManageBeginContactBtwPlayerAndDefaultPhysicsObj(ent_A);
		else if (ent_B)
			ManageBeginContactBtwPlayerAndDefaultPhysicsObj(ent_B);
	}

	void ContactListener::EndContact(b2Contact* contact)
	{
	}

}
