#include "ETpch.h"
#include <yaml-cpp/yaml.h>
#include <fstream>

#include "SceneSerializer.h"
#include "Eternal/Scene/Entity.h"
#include "Eternal/Scene/Component.h"

#include "Eternal/NativeScripts/ScriptHandler.h"


namespace YAML {

	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};

}

namespace Eternal {

	SceneSerializer::SceneSerializer(const SharedPtr<Scene>& scene)
		: m_Scene(scene)
	{
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	static void SerializeGroup(YAML::Emitter& out, const Group group)
	{
		out << YAML::BeginMap; //Group

		out << YAML::Key << "Group Name";
		out << YAML::Value << group.m_Name;

		out << YAML::Key << "Group ID";
		out << YAML::Value << group.m_ID;

		out << YAML::EndMap; //Group
	}

	static void SerializeEntity(YAML::Emitter& out, Entity entity)
	{
		out << YAML::BeginMap; //Entity

		out << YAML::Key << "Entity";
		out << YAML::Value << entity.GetEntityID(); //TODO: Random generated Entity IDs go here!

		if (entity.HasComponent<TagComponent>())
		{
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap; //TagComponent

			auto& tag = entity.GetComponent<TagComponent>().Tag;
			out << YAML::Key << "Tag" << YAML::Value << tag;
			
			auto& groupID = entity.GetComponent<TagComponent>().Group_ID;
			out << YAML::Key << "GroupID" << YAML::Value << groupID;
			out << YAML::EndMap; //TagComponent
		}

		if (entity.HasComponent<TransformComponent>())
		{
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap; // TransformComponent

			auto& tc = entity.GetComponent<TransformComponent>();
			out << YAML::Key << "Position" << YAML::Value << tc.Position;
			out << YAML::Key << "Rotation" << YAML::Value << tc.Rotation;
			out << YAML::Key << "Size" << YAML::Value << tc.Size;

			out << YAML::EndMap; // TransformComponent
		}

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			out << YAML::Key << "SpriteRendererComponent";
			out << YAML::BeginMap; // SpriteRendererComponent

			auto& spriteRendererComponent = entity.GetComponent<SpriteRendererComponent>();
			out << YAML::Key << "TextureFilepath" << YAML::Value << spriteRendererComponent.TextureFilepath;
			out << YAML::Key << "TextureScale" << YAML::Value << spriteRendererComponent.TextureScale;
			out << YAML::Key << "Color" << YAML::Value << spriteRendererComponent.Color;

			out << YAML::EndMap; // SpriteRendererComponent
		}

		if (entity.HasComponent<CameraComponent>())
		{
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap; // CameraComponent

			auto& cameraComponent = entity.GetComponent<CameraComponent>();
			auto& camera = cameraComponent.Camera;

			out << YAML::Key << "Camera" << YAML::Value;
			out << YAML::BeginMap; // Camera
			out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.GetProjectionType();
			out << YAML::Key << "PerspectiveFOV" << YAML::Value << camera.GetPerspectiveVerticalFOV();
			out << YAML::Key << "PerspectiveNear" << YAML::Value << camera.GetPerspectiveNearClip();
			out << YAML::Key << "PerspectiveFar" << YAML::Value << camera.GetPerspectiveFarClip();
			out << YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrthographicSize();
			out << YAML::Key << "OrthographicNear" << YAML::Value << camera.GetOrthographicNearClip();
			out << YAML::Key << "OrthographicFar" << YAML::Value << camera.GetOrthographicFarClip();
			out << YAML::EndMap; // Camera

			out << YAML::Key << "Primary" << YAML::Value << cameraComponent.Primary;
			out << YAML::Key << "FixedAspectRatio" << YAML::Value << cameraComponent.FixedAspectRatio;

			out << YAML::EndMap; // CameraComponent
		}

		if (entity.HasComponent<NativeScriptComponent>())
		{
			out << YAML::Key << "NativeScriptComponent";
			out << YAML::BeginMap; // NativeScriptComponent

			auto& nsc = entity.GetComponent<NativeScriptComponent>();
			out << YAML::Key << "ScriptFileName" << YAML::Value << nsc.ScriptName;

			out << YAML::EndMap; // NativeScriptComponent
		}

		if (entity.HasComponent<PhysicsComponent>())
		{
			out << YAML::Key << "PhysicsComponent";
			out << YAML::BeginMap; // PhysicsComponent

			auto& phc = entity.GetComponent<PhysicsComponent>();
			out << YAML::Key << "BodyShape" << YAML::Value << (int)phc.bodyShape;
			out << YAML::Key << "BodyType" << YAML::Value << phc.bodyType;
			out << YAML::Key << "Density" << YAML::Value << phc.fixtureDef.density;
			out << YAML::Key << "Friction" << YAML::Value << phc.fixtureDef.friction;
			out << YAML::Key << "Restitution" << YAML::Value << phc.fixtureDef.restitution;

			out << YAML::EndMap; // PhysicsComponent
		}

		if (entity.HasComponent<AudioComponent>())
		{
			out << YAML::Key << "AudioComponent";
			out << YAML::BeginMap; // AudioComponent

			auto& audiocomponent = entity.GetComponent<AudioComponent>();
			out << YAML::Key << "FileName" << YAML::Value << audiocomponent.filename;
			out << YAML::Key << "FilePath" << YAML::Value << audiocomponent.AudioFilePath;
			out << YAML::Key << "Gain" << YAML::Value << audiocomponent.Gain;
			out << YAML::Key << "Pitch" << YAML::Value << audiocomponent.Pitch;
			out << YAML::Key << "Looping" << YAML::Value << audiocomponent.Looping;

			out << YAML::EndMap; // AudioComponent
		}

		out << YAML::EndMap; //Entity
	}

	void SceneSerializer::Serialize(const std::string& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << m_Scene->GetName();

		out << YAML::Key << "Groups" << YAML::Value << YAML::BeginSeq;
		for each (Group group in m_Scene->m_Groups)
		{
			SerializeGroup(out, group);
		}
		out << YAML::EndSeq;

		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

		m_Scene->m_Registry.each([&](auto entityID)
			{
				Entity entity = { entityID, m_Scene.get() };
				if (!entity)
					return;

				SerializeEntity(out, entity);
			});
		out << YAML::EndSeq;
		
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
	}

	void SceneSerializer::SerializeRuntime(const std::string& filepath)
	{
		//Not implemented yet
		ET_CORE_ASSERT(false, "Not implemented yet!");
	}

	bool SceneSerializer::Deserialize(const std::string& filepath)
	{
		YAML::Node data = YAML::LoadFile(filepath);
		if (!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as<std::string>();
		ET_CORE_TRACE("Deserializing scene '{0}'", sceneName);
		m_Scene->SetName(sceneName);

		//Deserialize Groups
		auto groups = data["Groups"];
		if (groups)
		{
			for (auto group : groups)
			{
				std::string name = group["Group Name"].as<std::string>();
				uint32_t id = group["Group ID"].as<uint32_t>();
				auto n_group = Group(name, id);
				m_Scene->m_Groups.push_back(n_group);

				ET_CORE_TRACE("Deserialized group with ID = {0}, name = {1}", id, name);
			}
		}

		//Deserialize Entities
		auto entities = data["Entities"];
		if (entities)
		{
			for (auto entity : entities)
			{
				uint64_t uuid = entity["Entity"].as<uint64_t>(); // TODO randomize IDs...

				std::string name;
				auto tagComponent = entity["TagComponent"];
				if (tagComponent)
					name = tagComponent["Tag"].as<std::string>();

				ET_CORE_TRACE("Deserialized entity with ID = {0}, name = {1}", uuid, name);

				Entity deserializedEntity = m_Scene->CreateEntity(name);

				if (tagComponent)
				{
					auto& tagc = deserializedEntity.GetComponent<TagComponent>();
					tagc.Group_ID = tagComponent["GroupID"].as<int>();
				}

				auto transformComponent = entity["TransformComponent"];
				if (transformComponent)
				{
					// Entities always have transforms
					auto& tc = deserializedEntity.GetComponent<TransformComponent>();
					tc.Position = transformComponent["Position"].as<glm::vec3>();
					tc.Rotation = transformComponent["Rotation"].as<float>();
					tc.Size = transformComponent["Size"].as<glm::vec2>();
				}

				auto spriteRendererComponent = entity["SpriteRendererComponent"];
				if (spriteRendererComponent)
				{
					auto& src = deserializedEntity.AddComponent<SpriteRendererComponent>();
					src.TextureFilepath = spriteRendererComponent["TextureFilepath"].as<std::string>();
					if(src.TextureFilepath.compare(""))
						src.Texture = Texture2D::Create(src.TextureFilepath); //only create a texture if there is one :)
					src.TextureScale = spriteRendererComponent["TextureScale"].as<int>();
					src.Color = spriteRendererComponent["Color"].as<glm::vec4>();
				}

				auto cameraComponent = entity["CameraComponent"];
				if (cameraComponent)
				{
					auto& cc = deserializedEntity.AddComponent<CameraComponent>();

					auto& cameraProps = cameraComponent["Camera"];

					cc.Camera.SetProjectionType((SceneCamera::ProjectionType)cameraProps["ProjectionType"].as<int>());

					cc.Camera.SetPerspectiveVerticalFOV(cameraProps["PerspectiveFOV"].as<float>());
					cc.Camera.SetPerspectiveNearClip(cameraProps["PerspectiveNear"].as<float>());
					cc.Camera.SetPerspectiveFarClip(cameraProps["PerspectiveFar"].as<float>());

					cc.Camera.SetOrthographicSize(cameraProps["OrthographicSize"].as<float>());
					cc.Camera.SetOrthographicNearClip(cameraProps["OrthographicNear"].as<float>());
					cc.Camera.SetOrthographicFarClip(cameraProps["OrthographicFar"].as<float>());

					cc.Primary = cameraComponent["Primary"].as<bool>();
					cc.FixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();
				}

				auto nativeScriptComponent = entity["NativeScriptComponent"];
				if (nativeScriptComponent)
				{
					auto& nsc = deserializedEntity.AddComponent<NativeScriptComponent>();
					nsc.ScriptName = nativeScriptComponent["ScriptFileName"].as<std::string>();
					auto scriptNumber = ScriptHandler::GetScriptNumber(nsc.ScriptName);
					nsc.SetScript(ScriptHandler::GetScript(scriptNumber));
				}

				auto physicsComponent = entity["PhysicsComponent"];
				if (physicsComponent)
				{
					auto& phc = deserializedEntity.AddComponent<PhysicsComponent>();
					phc.bodyShape = (PhysicsComponent::BodyShape)physicsComponent["BodyShape"].as<int>();
					phc.bodyType = (b2BodyType)physicsComponent["BodyType"].as<int>();
					phc.fixtureDef.density = physicsComponent["Density"].as<float>();
					phc.fixtureDef.friction = physicsComponent["Friction"].as<float>();
					phc.fixtureDef.restitution = physicsComponent["Restitution"].as<float>();
				}

				auto audioComponent = entity["AudioComponent"];
				if (audioComponent)
				{
					auto& auc = deserializedEntity.AddComponent<AudioComponent>();
					auc.filename = audioComponent["FileName"].as<std::string>();
					auc.AudioFilePath = audioComponent["FilePath"].as<std::string>();
					auc.LoadFile();
					auc.Gain = audioComponent["Gain"].as<float>();
					auc.Pitch = audioComponent["Pitch"].as<float>();
					auc.Looping = audioComponent["Looping"].as<bool>();
				}
			}
		}

		return true;
	}

	bool SceneSerializer::DeserializeRuntime(const std::string& filepath)
	{
		//Not implemented yet
		ET_CORE_ASSERT(false, "Not implemented yet!");
		return false;
	}
}