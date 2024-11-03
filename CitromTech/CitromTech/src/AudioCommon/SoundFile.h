#pragma once

#include "Core.h"
#include "FileSystem/FileSystem.h"

#include "CTL/SmartPointers.h"

// this is supposed to replace libsndfile since i
// want to dynamically link libraries as little as possible
namespace Citrom
{
	struct SoundFileInfo
	{
		CTL::ScopedPtr<float32> samples;
		uint64 sampleCount;
		uint32 channels;
		uint32 sampleRate;
	};

	class SoundFile
	{
	public:
		enum class Type
		{
			Unknown,
			WAV,
			MP3,
			FLAC,
			OGG
		};
	public:
		static SoundFile* Open(const FileSystem::FilePath& path, SoundFileInfo& outInfo);
		//static CTL::ScopedPtr<SoundFile> Open(const FileSystem::FilePath& path, SoundFileInfo& outInfo);

		static void Close(SoundFile* soundFile);

		static uint64 ReadPCMFramesFloat32(SoundFile* soundFile, const SoundFileInfo& info, uint64 framesToRead, uint32 channelCount, float32* bufferOut);
		static void SeekStart(SoundFile* soundFile);
		static void SeekFrame(SoundFile* soundFile, uint64 frame);
	private:
		void* m_Internal = nullptr;
		Type m_FileType = Type::Unknown;
	};
}
typedef enum port_audio_file_type
{
	FILE_TYPE_UNKNOWN,
	FILE_TYPE_WAV,
	FILE_TYPE_MP3,
	FILE_TYPE_FLAC,
	FILE_TYPE_OGG,
	NUM_FILE_TYPES
} port_audio_file_type;

typedef struct port_audio_sound_file
{
	void* internal;
	port_audio_file_type fileType;
} port_audio_sound_file;

typedef struct port_audio_sound_file_info
{
	float32* samples;
	uint64 sampleCount;
	uint32 channels;
	uint32 sampleRate;
} port_audio_sound_file_info;

typedef enum port_audio_sound_file_mode
{
	PORT_AUDIO_SOUND_FILE_MODE_READ,
	NUM_PORT_AUDIO_SOUND_FILE_MODES
} port_audio_sound_file_mode;

port_audio_sound_file* port_audio_sound_file_open(const char* path, const port_audio_sound_file_mode mode, port_audio_sound_file_info* info);
uint64 port_audio_read_pcm_frames_float32(port_audio_sound_file* soundFile, port_audio_sound_file_info* soundInfo, uint64 framesToRead, uint32 channelCount, float32* bufferOut);
void port_audio_seek_start(port_audio_sound_file* soundFile);
void port_audio_seek_frame(port_audio_sound_file* soundFile, uint64 frame);
void port_audio_sound_file_close(port_audio_sound_file* soundFile, port_audio_sound_file_info* soundInfo);