#pragma once

#include "Scene.h"

namespace Citrom
{
	class SceneSerializer
	{
	public:
		SceneSerializer(Scene* scene);

		void Serialize(const std::string& filePath);
		bool Deserialize(const std::string& filePath);

		// TODO: SerializeBinary and DeserializeBinary
	private:
		Scene* m_Scene;
	};
}