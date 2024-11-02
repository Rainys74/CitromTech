#include "AudioSystem.h"
#include "AudioCommon/SoundFile.h"

#include "CitromAssert.h"
#include "CitromMemory.h"

#include "portaudio.h"

namespace Citrom::Audio
{
    constexpr unsigned long FRAMES_PER_BUFFER = 512;

    struct CallbackData
    {
        SoundFile* file;
        SoundFileInfo info;

        Clip* userClip;
    };

    static void PortAudioCallback(const void* input, void* output, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData);

    int GetDeviceCount()
    {
        return Pa_GetDeviceCount();
    }

    void FillDeviceAtIndex(Device& deviceInfoToFill, int index)
    {
        const PaDeviceInfo* deviceInfo;

        deviceInfo = Pa_GetDeviceInfo(index);
        CT_CORE_ASSERT(deviceInfo, "Failed to retrieve Device Info from PortAudio!");

        deviceInfoToFill.index = index;
        deviceInfoToFill.name = const_cast<char*>(deviceInfo->name);
    }

    void Initialize()
    {
        CT_CORE_ASSERT(Pa_Initialize() == paNoError, "Problem initializing PortAudio");
    }

    void Deinitialize()
    {
        CT_CORE_ASSERT(Pa_Terminate() == paNoError, "Problem terminating PortAudio");
    }

    const char* GetBackendName()
    {
        const PaDeviceInfo* deviceInfo = Pa_GetDeviceInfo(Pa_GetDefaultOutputDevice());
        CT_CORE_ASSERT(deviceInfo, "Failed to get default output device info!");
        const PaHostApiInfo* hostApiInfo = Pa_GetHostApiInfo(deviceInfo->hostApi);
        CT_CORE_ASSERT(deviceInfo, "Failed to get host api info!");

        return hostApiInfo->name;
    }

    void PlayAudioClip(Clip* clip)
    {
        /*clip->shouldPlay = true;

        CallbackData data = { 0 };
        //data.file = port_audio_sound_file_open(clip->path, PORT_AUDIO_SOUND_FILE_MODE_READ, &data.info);
        data.file = new SoundFile(clip->path, SoundFile::Mode::Read, data.info);
        data.userClip = clip;

        CT_CORE_INFO("File Type: {int}", data.file->fileType);
        CT_CORE_INFO("Channels: {uint}", data.info.channels);
        CT_CORE_INFO("Sample Rate: {uint}", data.info.sampleRate);
        CT_CORE_INFO("Sample Count: {uint}", data.info.sampleCount);

        for (size_t i = 0; i < GetDeviceCount(); i++)
        {
            Device device;

            FillDeviceAtIndex(&device, i);

            CT_CORE_INFO("Device {int}:", device.index);
            CT_CORE_INFO("\tName: {cstr}", device.name);
        }

        PaStream* stream;

        CT_CORE_ASSERT(Pa_OpenDefaultStream(&stream, 0, data.info.channels, paFloat32, data.info.sampleRate, FRAMES_PER_BUFFER, PortAudioCallback, &data) == paNoError, "Problem opening PortAudio Default Stream.");
        CT_CORE_ASSERT(Pa_StartStream(stream) == paNoError, "Problem opening PortAudio starting Stream.");

        // Run until EOF is reached
        while (Pa_IsStreamActive(stream))
        {
            Pa_Sleep(100);
        }

        port_audio_sound_file_close(data.file, &data.info);

        CT_CORE_ASSERT(Pa_CloseStream(stream) == paNoError, "Problem closing PortAudio stream");

        clip->shouldPlay = false;*/
    }
    /*static void PortAudioCallback(const void* input, void* output, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData)
    {
        float* fOutput = (float*)output;
        CallbackData* data = (CallbackData*)userData;

        Memory::Set(fOutput, 0, sizeof(float32) * frameCount * data->info.channels);

        // read directly into output buffer
        uint64 framesRead = port_audio_read_pcm_frames_float32(data->file, &data->info, frameCount, data->info.channels, fOutput);
        //CT_MOD_ERROR("audio callback");

        // If we couldn't read a full frameCount of samples, we've reached EOF
        if (framesRead < frameCount)
        {
            if (data->userClip->looping)
            {
                port_audio_seek_start(data->file);
                return paContinue;
            }
            else
            {
                return paComplete;
            }
        }

        // Adjust volume of the audio data
        for (uint64fast i = 0; i < frameCount * data->info.channels; ++i)
        {
            fOutput[i] *= data->userClip->volume;
        }

        // return paContinue;
        if (!data->userClip->isPlaying)
        {
            return paComplete;
        }
        else
        {
            return paContinue;
        }
    }*/
}