#ifndef RAGE_SOUND_DRIVER_MINIAUDIO_H
#define RAGE_SOUND_DRIVER_MINIAUDIO_H

#include "RageSoundDriver.h"
#include "miniaudio.h"

class RageSoundDriver_MiniAudio : public RageSoundDriver
{
public:
	RageSoundDriver_MiniAudio();
	virtual ~RageSoundDriver_MiniAudio();

	virtual RString Init();
	virtual int64_t GetPosition() const;
	virtual float GetPlayLatency() const;
	virtual int GetSampleRate() const;

private:
	ma_device m_Device;
	ma_device_config m_Config;
	int m_iSampleRate;
	int64_t m_iCurrentFrame;

	static void DataCallback( ma_device *pDevice, void *pOutput, const void *pInput, ma_uint32 frameCount );
};

#endif
