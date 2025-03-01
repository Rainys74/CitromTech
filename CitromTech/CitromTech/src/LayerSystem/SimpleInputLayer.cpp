#include "SimpleInputLayer.h"

namespace Citrom
{
	using namespace Input;

	void SimpleInputLayer::OnAttach()
	{
		(void)SimpleInputManager::GetInstance();
	}

	void SimpleInputLayer::OnUpdate(float64 deltaTime)
	{
		SimpleInputManager::GetInstance()->Update();
	}
}