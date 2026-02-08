#include "global.h"
#include "RageSoundDriver_MiniAudio.h"
#include "RageLog.h"
#include "PrefsManager.h"

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

REGISTER_SOUND_DRIVER_CLASS( MiniAudio );

void RageSoundDriver_MiniAudio::DataCallback( ma_device *pDevice, void *pOutput, const void *pInput, ma_uint32 frameCount )
{
	RageSoundDriver_MiniAudio *pDriver = (RageSoundDriver_MiniAudio *)pDevice->pUserData;
	pDriver->Mix( (float *)pOutput, frameCount, pDriver->m_iCurrentFrame, pDriver->GetPosition() );
	pDriver->m_iCurrentFrame += frameCount;
}

RageSoundDriver_MiniAudio::RageSoundDriver_MiniAudio()
{
	m_iCurrentFrame = 0;
	m_iSampleRate = 44100;
}

RageSoundDriver_MiniAudio::~RageSoundDriver_MiniAudio()
{
	ma_device_uninit( &m_Device );
}

RString RageSoundDriver_MiniAudio::Init()
{
	ma_device_config config = ma_device_config_init( ma_device_type_playback );
	config.playback.format   = ma_format_f32;
	config.playback.channels = 2;
	config.sampleRate        = PREFSMAN->m_iSoundPreferredSampleRate;
	config.dataCallback      = DataCallback;
	config.pUserData         = this;

	if (ma_device_init( NULL, &config, &m_Device ) != MA_SUCCESS)
	{
		return "Failed to initialize miniaudio device.";
	}

	m_iSampleRate = m_Device.sampleRate;
	
	StartDecodeThread();

	if (ma_device_start( &m_Device ) != MA_SUCCESS)
	{
		return "Failed to start miniaudio device.";
	}

	return "";
}

int64_t RageSoundDriver_MiniAudio::GetPosition() const
{
	ma_uint64 time;
	// Cast away constness because miniaudio API takes non-const pointer, 
	// though it probably doesn't modify the device state in a way that matters for us here.
	// Actually ma_device_get_time_in_pcm_frames takes const ma_device* in the header I read.
	// Let's check the header content I fetched.
	// "MA_API ma_result ma_device_get_time_in_pcm_frames(const ma_device* pDevice, ma_uint64* pTime);"
	// So it is const.
	if (ma_device_get_time_in_pcm_frames( &m_Device, &time ) == MA_SUCCESS)
		return (int64_t)time;
	return m_iCurrentFrame;
}

float RageSoundDriver_MiniAudio::GetPlayLatency() const
{
	return (float)ma_device_get_latency_in_pcm_frames( &m_Device ) / m_iSampleRate;
}

int RageSoundDriver_MiniAudio::GetSampleRate() const
{
	return m_iSampleRate;
}
