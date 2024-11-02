#pragma once

#include "Core.h"

// There's no reason to support Audio API's because
// PortAudio should be the only one that needs to be used
namespace Citrom::Audio
{
	struct Device
	{
		int index;
		char* name;
	};
	struct Clip
	{
		uint8* data; // bytes
		uint32 dataLength;
		
		bool looping;
		float32 volume;

		Device* customDevice; // pass in nullptr if you want to use the default
		bool shouldPlay;
	};

	int GetDeviceCount();
	void FillDeviceAtIndex(Device& deviceInfoToFill, int index);

	void Initialize();
	void Deinitialize();

	const char* GetBackendName();

	void PlayAudioClip(Clip* clip);
	//void PlayAudioClipAsync(Clip* clip); // ThreadPool* customThreadPool
}