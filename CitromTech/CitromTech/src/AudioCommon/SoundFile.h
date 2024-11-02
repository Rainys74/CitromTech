#pragma once

#include "Core.h"

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
		enum class Mode
		{
			Read
		};
	public:
		SoundFile(const char* path, const Mode mode, SoundFileInfo& info)
			: m_Internal(nullptr), m_FileType(Type::Unknown) {} // Open
		virtual ~SoundFile() {} // Close

		virtual uint64 ReadPCMFramesFloat32(SoundFileInfo& soundInfo, uint64 framesToRead, uint32 channelCount, float32* bufferOut) = 0;
		virtual void SeekStart() = 0;
		virtual void SeekFrame(uint64 frame) = 0;

		virtual void* Data() {return m_Internal;}
	private:
		void* m_Internal = nullptr;
		Type m_FileType = Type::Unknown;
	};

	class SoundFileWAV : public SoundFile
	{
	public:
		SoundFileWAV(const char* path, const Mode mode, SoundFileInfo& info);
		virtual ~SoundFileWAV();

		virtual uint64 ReadPCMFramesFloat32(SoundFileInfo& soundInfo, uint64 framesToRead, uint32 channelCount, float32* bufferOut) override;
		virtual void SeekStart() override;
		virtual void SeekFrame(uint64 frame) override;

		virtual void* Data() override { return m_Internal; }
	private:
		void* m_Internal = nullptr;
		Type m_FileType = Type::WAV;
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