#pragma once

#include "Core.h"

// Make a mechanism that returns the count of "standalone threads" so the thread pool doesn't cause the creation of too many threads and thus slowing down the app

int SharedMain(int argc, char* argv[]);
// Main Loop: ApplicationLoop, RuntimeLoop, ExecutionLoop, EngineLoop, GameLoop, CoreLoop, ForgeLoop (somewhat inspired by D_DoomLoop) best: ForgeLoop, GameLoop, RuntimeLoop, ApplicationLoop, MainLoop, SharedLoop.
void ForgeLoop();
// RenderLoop (Render Thread Main Loop) (Initializes the renderer and Renders the submitted scene data whenever submitting has finished, as in Renderer::End was called)

float64 MainDeltaTime();
float64 MainFPS();

void* GetCurrentScene();

void* GetCamera();
void* GetCameraTransform();