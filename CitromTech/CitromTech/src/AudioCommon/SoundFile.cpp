#include "SoundFile.h"

#include "Vendor/dr_libs/dr_wav.h"
#include "Vendor/dr_libs/dr_mp3.h"
#include "Vendor/dr_libs/dr_flac.h"

#include "Vendor/stb/stb_vorbis.h"

#include "CTL/CStringHandling.h"
#include "Logger/Logger.h"
#include "CitromMemory.h"
#include "CitromAssert.h"

#include <string.h>

namespace Citrom
{
#define dr_lib_sound_file_open(LIBTYPE, FILETYPE, PCMFRAMETOTALNAME)                                \
    file->fileType = FILETYPE;                                                                      \
    file->internal = new dr ## LIBTYPE;                                                     \
    dr ## LIBTYPE* LIBTYPE = (dr ## LIBTYPE*)file->internal;                                        \
                                                                                                    \
    CT_CORE_ASSERT(dr ## LIBTYPE ## _init_file(LIBTYPE, path, NULL), "drwav_init_file() failed!");   \
    CT_CORE_ASSERT(LIBTYPE, "Could not open " #LIBTYPE " file: " "!");                  \
                                                                                                    \
    info->channels = LIBTYPE->channels;                                                             \
    info->sampleCount = LIBTYPE->PCMFRAMETOTALNAME;                                                 \
    info->sampleRate = LIBTYPE->sampleRate                                                             

    SoundFileWAV::SoundFileWAV(const char* path, const Mode mode, SoundFileInfo& info) 
        : SoundFile(path, mode, info), m_Internal(nullptr), m_FileType(Type::WAV)
    {
        m_Internal = new drwav;
        drwav* wav = (drwav*)m_Internal;

        CT_CORE_ASSERT(drwav_init_file(wav, path, NULL), "drwav_init_file() failed!");
        CT_CORE_ASSERT(wav, "Could not open wav file!");

        info.channels = wav->channels;
        info.sampleCount = wav->totalPCMFrameCount;
        info.sampleRate = wav->sampleRate;
    }
    SoundFileWAV::~SoundFileWAV()
    {
        drwav_uninit((drwav*)m_Internal);

        delete (drwav*)m_Internal;
    }
    uint64 SoundFileWAV::ReadPCMFramesFloat32(SoundFileInfo& soundInfo, uint64 framesToRead, uint32 channelCount, float32* bufferOut)
    {
        return drwav_read_pcm_frames_f32((drwav*)m_Internal, framesToRead, bufferOut);
    }
    void SoundFileWAV::SeekStart()
    {
        drwav_seek_to_pcm_frame((drwav*)m_Internal, 0);
    }
    void SoundFileWAV::SeekFrame(uint64 frame)
    {
        // to be implemented
    }



    static port_audio_file_type get_file_type(const char* fileName)
    {
        const char* extension = CTL::CString::ReverseSearchForCharacter(fileName, '.');
        if (extension != NULL)
        {
            // Move to the next character after the dot
            extension++;
            if (strcmp(extension, "wav") == 0)
            {
                return FILE_TYPE_WAV;
            }
            else if (strcmp(extension, "ogg") == 0)
            {
                return FILE_TYPE_OGG;
            }
            else if (strcmp(extension, "mp3") == 0)
            {
                return FILE_TYPE_MP3;
            }
            else if (strcmp(extension, "flac") == 0)
            {
                return FILE_TYPE_FLAC;
            }
        }
        return FILE_TYPE_UNKNOWN;
    }

    port_audio_sound_file* port_audio_sound_file_open(const char* path, const port_audio_sound_file_mode mode, port_audio_sound_file_info* info)
    {
        port_audio_sound_file* file = new port_audio_sound_file;
        switch (get_file_type(path))
        {
        default:
        case FILE_TYPE_UNKNOWN:
            CT_CORE_ERROR("port_audio_sound_file_open(): Cannot open sound file: {cstr}!");
            break;
        case FILE_TYPE_WAV:
        {
            dr_lib_sound_file_open(wav, FILE_TYPE_WAV, totalPCMFrameCount);
        }
        break;
        case FILE_TYPE_MP3:
        {
            dr_lib_sound_file_open(mp3, FILE_TYPE_MP3, pcmFramesRemainingInMP3Frame);
        }
        break;
        case FILE_TYPE_FLAC:
        {
            file->fileType = FILE_TYPE_FLAC;
            file->internal = drflac_open_file(path, NULL);
            drflac* flac = (drflac*)file->internal;

            CT_CORE_ASSERT(flac, "Could not open flac file: {cstr}!");

            info->channels = flac->channels;
            info->sampleCount = flac->totalPCMFrameCount;
            info->sampleRate = flac->sampleRate;
        }
        break;
        case FILE_TYPE_OGG:
        {
            file->fileType = FILE_TYPE_OGG;
            file->internal = stb_vorbis_open_filename(path, NULL, NULL);
            stb_vorbis* vorbis = (stb_vorbis*)file->internal;

            CT_CORE_ASSERT(vorbis, "Could not open ogg file: {cstr}!");

            int channels, sampleRate;
            short* samples;
            info->samples = (float32*)stb_vorbis_decode_filename(path, &channels, &sampleRate, &samples);

            CT_CORE_ASSERT(info->samples, "Could not decode ogg file: {cstr}!");

            info->channels = channels;
            info->sampleCount = stb_vorbis_stream_length_in_samples(vorbis);
            info->sampleRate = sampleRate;
            // TODO: maybe free?
        }
        break;
        }
        return file;
    }

    uint64 port_audio_read_pcm_frames_float32(port_audio_sound_file* soundFile, port_audio_sound_file_info* soundInfo, uint64 framesToRead, uint32 channelCount, float32* bufferOut)
    {
        switch (soundFile->fileType)
        {
        default:
        case FILE_TYPE_UNKNOWN:
            CT_CORE_ERROR("port_audio_read_pcm_frames_float32(): Cannot read pcm frames!");
            return 0;
            break;
        case FILE_TYPE_WAV:
        {
            return drwav_read_pcm_frames_f32((drwav*)soundFile->internal, framesToRead, bufferOut);
        }
        break;
        case FILE_TYPE_MP3:
        {
            return drmp3_read_pcm_frames_f32((drmp3*)soundFile->internal, framesToRead, bufferOut);
        }
        break;
        case FILE_TYPE_FLAC:
        {
            return drflac_read_pcm_frames_f32((drflac*)soundFile->internal, framesToRead, bufferOut);
        }
        break;
        case FILE_TYPE_OGG:
        {
            return stb_vorbis_get_samples_float_interleaved((stb_vorbis*)soundFile->internal, soundInfo->channels, bufferOut, framesToRead * channelCount);
        }
        break;
        }
    }

    void port_audio_seek_start(port_audio_sound_file* soundFile)
    {
        switch (soundFile->fileType)
        {
        default:
        case FILE_TYPE_UNKNOWN:
            CT_CORE_ERROR("port_audio_seek_start(): Cannot seek start!");
            break;
        case FILE_TYPE_WAV:
        {
            drwav_seek_to_pcm_frame((drwav*)soundFile->internal, 0);
        }
        break;
        case FILE_TYPE_MP3:
        {
            drmp3_seek_to_pcm_frame((drmp3*)soundFile->internal, 0);
        }
        break;
        case FILE_TYPE_FLAC:
        {
            drflac_seek_to_pcm_frame((drflac*)soundFile->internal, 0);
        }
        break;
        case FILE_TYPE_OGG:
        {
            stb_vorbis_seek_start((stb_vorbis*)soundFile->internal);
        }
        break;
        }
    }

    void port_audio_seek_frame(port_audio_sound_file* soundFile, uint64 frame)
    {
        // TODO: to be implemented
    }

    void port_audio_sound_file_close(port_audio_sound_file* soundFile, port_audio_sound_file_info* soundInfo)
    {
        switch (soundFile->fileType)
        {
        default:
        case FILE_TYPE_UNKNOWN:
            CT_CORE_ERROR("port_audio_sound_file_close(): Cannot close sound file!");
            break;
        case FILE_TYPE_WAV:
        {
            drwav_uninit((drwav*)soundFile->internal);

            delete (drwav*)soundFile->internal;
        }
        break;
        case FILE_TYPE_MP3:
        {
            drmp3_uninit((drmp3*)soundFile->internal);

            delete (drmp3*)soundFile->internal;
        }
        break;
        case FILE_TYPE_FLAC:
        {
            drflac_close((drflac*)soundFile->internal);
        }
        break;
        case FILE_TYPE_OGG:
        {
            stb_vorbis_close((stb_vorbis*)soundFile->internal);
        }
        break;
        }

        delete soundFile;
    }
}