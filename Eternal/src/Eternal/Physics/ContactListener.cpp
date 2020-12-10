#include "ETpch.h"
#include "ContactListener.h"

#include "Eternal/NativeScripts/Deathbox.h"

namespace Eternal {

	void ManageBeginContact(ScriptableEntity* ent_A, ScriptableEntity* ent_B)
	{
		auto scriptType_A = ent_A->GetScriptType();
		auto scriptType_B = ent_B->GetScriptType();

		switch (scriptType_A)
		{
		case ScriptType::default:
			break;
		case ScriptType::player:
			ET_CORE_INFO("A is player");
			break;
		case ScriptType::deathbox:
			ET_CORE_INFO("A is deathbox");
			if(scriptType_B == ScriptType::player)
				ent_A->HandleContact();
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
			break;
		case ScriptType::deathbox:
			ET_CORE_INFO("B is deathbox");
			if(scriptType_A == ScriptType::player)
				ent_B->HandleContact();
			break;
		default:
			break;
		}
	}

	void ContactListener::BeginContact(b2Contact* contact)
	{
		ScriptableEntity* ent_A = reinterpret_cast<ScriptableEntity*>(contact->GetFixtureA()->GetBody()->GetUserData().pointer);
		ScriptableEntity* ent_B = reinterpret_cast<ScriptableEntity*>(contact->GetFixtureB()->GetBody()->GetUserData().pointer);
		if (ent_A && ent_B)
			ManageBeginContact(ent_A, ent_B);
	}

	void ContactListener::EndContact(b2Contact* contact)
	{
	}

}
