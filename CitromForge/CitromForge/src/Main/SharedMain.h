#pragma once

#include "Core.h"

// Make a mechanism that returns the count of "standalone threads" so the thread pool doesn't cause the creation of too many threads and thus slowing down the app

int SharedMain(int argc, char* argv[]);
// Main Loop: ApplicationLoop, RuntimeLoop, ExecutionLoop, EngineLoop, GameLoop, CoreLoop, ForgeLoop (somewhat inspired by D_DoomLoop) best: ForgeLoop, GameLoop, RuntimeLoop, ApplicationLoop, MainLoop, SharedLoop.
void ForgeLoop();
// RenderLoop (Render Thread Main Loop) (Initializes the renderer and Submits (RenderPrep) and renders the data, at the start copy over scene data so you don't start rendering garbage (IB: Unreal))

float64 MainDeltaTime();
float64 MainFPS();

namespace Citrom
{
	class Scene;
}

Citrom::Scene* GetCurrentScene();

namespace Citrom
{
	struct Camera; // should be class
	namespace Math
	{
		struct Transform;
	}
	namespace Platform
	{
		class Window;
	}
}

Citrom::Camera* GetCamera();
Citrom::Math::Transform* GetCameraTransform();

Citrom::Platform::Window* GetMainWindow();