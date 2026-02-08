#include "global.h"
#include "RageSoundDriver_Null.h"
#include "RageLog.h"
#include "RageUtil.h"
<<<<<<< HEAD:itgmania/src/arch/Sound/RageSoundDriver_Null.cpp
#include "PrefsManager.h"
=======

#include "arch/arch_default.h"
>>>>>>> origin/c++11:src/arch/Sound/RageSoundDriver.cpp

#include <cstdint>

REGISTER_SOUND_DRIVER_CLASS( Null );

const int channels = 2;

void RageSoundDriver_Null::Update()
{
<<<<<<< HEAD:itgmania/src/arch/Sound/RageSoundDriver_Null.cpp
	/* "Play" frames. */
	while( m_iLastCursorPos < GetPosition()+1024*4 )
	{
		int16_t buf[256*channels];
		this->Mix( buf, 256, m_iLastCursorPos, GetPosition() );
		m_iLastCursorPos += 256;
	}

	RageSoundDriver::Update();
}

int64_t RageSoundDriver_Null::GetPosition() const
{
	return (RageTimer::GetTimeSinceStartMicroseconds() * m_iSampleRate) / 1000000;
}

RageSoundDriver_Null::RageSoundDriver_Null()
{
	m_iSampleRate = PREFSMAN->m_iSoundPreferredSampleRate;
	{
    	m_iSampleRate = kFallbackSampleRate;
	}
	m_iLastCursorPos = GetPosition();
	StartDecodeThread();
}

int RageSoundDriver_Null::GetSampleRate() const
{
	return m_iSampleRate;
=======
	vector<RString> DriversToTry;
	split( sDrivers.empty()? DEFAULT_SOUND_DRIVER_LIST:sDrivers, ",", DriversToTry, true );
	
	for (RString const &Driver : DriversToTry)
	{
		RageDriver *pDriver = m_pDriverList.Create( Driver );
		char const * driverString = Driver.c_str();
		if( pDriver == nullptr )
		{
			LOG->Trace( "Unknown sound driver: %s", driverString );
			continue;
		}

		RageSoundDriver *pRet = dynamic_cast<RageSoundDriver *>( pDriver );
		ASSERT( pRet != nullptr );

		const RString sError = pRet->Init();
		if( sError.empty() )
		{
			LOG->Info( "Sound driver: %s", driverString );
			return pRet;
		}
		LOG->Info( "Couldn't load driver %s: %s", driverString, sError.c_str() );
		SAFE_DELETE( pRet );
	}
	return nullptr;
>>>>>>> origin/c++11:src/arch/Sound/RageSoundDriver.cpp
}

/*
 * (c) 2002-2004 Glenn Maynard, Aaron VonderHaar
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, and/or sell copies of the Software, and to permit persons to
 * whom the Software is furnished to do so, provided that the above
 * copyright notice(s) and this permission notice appear in all copies of
 * the Software and that both the above copyright notice(s) and this
 * permission notice appear in supporting documentation.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT OF
 * THIRD PARTY RIGHTS. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR HOLDERS
 * INCLUDED IN THIS NOTICE BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL INDIRECT
 * OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
 * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */
