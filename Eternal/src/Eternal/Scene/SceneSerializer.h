#pragma once

#include "Scene.h"

namespace Eternal {

	class SceneSerializer
	{
	public:
		SceneSerializer(const SharedPtr<Scene>& scene);

		void Serialize(const std::string& filepath);
		void SerializeRuntime(const std::string& filepath);

		bool Deserialize(const std::string& filepath);
		bool DeserializeRuntime(const std::string& filepath);
	private:
		SharedPtr<Scene> m_Scene;
	};
}