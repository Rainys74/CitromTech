#pragma once

#include "Entity.h"

namespace Citrom
{
	// Scriptable Entity Game Object
	class ScriptableEntity
	{
	public:
		template<typename T>
		inline T& GetComponent()
		{
			CT_CORE_ASSERT(m_Entity.HasComponent<T>(), "Entity does not have the following component!"); // Scriptable Entities should have flawless error-checking for the sake of the user
			return m_Entity.GetComponent<T>();
		}

		inline Scene* GetScene()
		{
			CT_CORE_ASSERT(m_Entity && m_Entity.GetScene(), "Either Entity is null or the Current Scene is null!");
			return m_Entity.GetScene();
		}

		//inline Entity& FindByName(const std::string& name) {}
	private:
		Entity m_Entity;

		friend class Scripting::NativeScript;
	};
}