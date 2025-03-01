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
			return m_Entity.GetComponent<T>();
		}
	private:
		Entity m_Entity;

		friend class Scripting::NativeScript;
	};
}