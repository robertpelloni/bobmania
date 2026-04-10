#include "global.h"
#include "MovieTexture_Generic.h"
#include "PrefsManager.h"
#include "RageDisplay.h"
#include "RageLog.h"
#include "RageSurface.h"
#include "RageTextureManager.h"
#include "RageTextureRenderTarget.h"
#include "RageTimer.h"
#include "RageUtil.h"
#include "Sprite.h"

#include <cmath>
#include <cstdint>

#if defined(WIN32)
#include "archutils/Win32/ErrorStrings.h"
#include <windows.h>
#endif


static Preference<bool> g_bMovieTextureDirectUpdates("MovieTextureDirectUpdates", true);

MovieTexture_Generic::MovieTexture_Generic(RageTextureID ID, std::unique_ptr<MovieDecoder> pDecoder) :
	RageMovieTexture(ID)
{
	LOG->Trace("MovieTexture_Generic::MovieTexture_Generic(%s)", ID.filename.c_str());

	decoder_ = std::move(pDecoder);

<<<<<<< HEAD
	texture_handle_ = 0;
	render_target_ = nullptr;
	intermediate_texture_ = nullptr;
	loop_ = true;
	surface_ = nullptr;
	texture_lock_ = nullptr;
	rate_ = 1;
	clock_ = 0;
	sprite_ = std::make_unique<Sprite>();
=======
	m_uTexHandle = 0;
	m_pRenderTarget = nullptr;
	m_pTextureIntermediate = nullptr;
	m_bLoop = true;
	m_pSurface = nullptr;
	m_pTextureLock = nullptr;
	m_ImageWaiting = FRAME_NONE;
	m_fRate = 1;
	m_bWantRewind = false;
	m_fClock = 0;
	m_bFrameSkipMode = false;
	m_pSprite = new Sprite;
>>>>>>> main
}

RString MovieTexture_Generic::Init()
{
	RString err = decoder_->Open(GetID().filename);
	if (err != "") {
		LOG->Warn("MovieTexture_Generic::Init: failed to open decoder for file: %s, with error:\n%s", GetID().filename.c_str(), err.c_str());
		return err;
	}

	CreateTexture();
	CreateFrameRects();
	decoder_->SetLooping(loop_);

	decoding_thread_ = std::make_unique<std::thread>([this]() {
		LOG->Trace("Beginning to decode video file \"%s\"", GetID().filename.c_str());
		auto timer = RageTimer();

		int ret = decoder_->DecodeMovie();
		if (ret == -1) {
			failure_ = true;
		}

		LOG->Trace("Done decoding video file \"%s\", took %f seconds", GetID().filename.c_str(), timer.Ago());
		});

	LOG->Trace("Resolution: %ix%i (%ix%i, %ix%i)",
		m_iSourceWidth, m_iSourceHeight,
		m_iImageWidth, m_iImageHeight, m_iTextureWidth, m_iTextureHeight);

	CHECKPOINT;

	return RString();
}

MovieTexture_Generic::~MovieTexture_Generic()
{
	if (decoder_) {
		decoder_->Cancel();
		decoding_thread_->join();
		decoder_->Close();
	}

	/* sprite_ may reference the texture; delete it before DestroyTexture. */
	sprite_.reset();
	DestroyTexture();

}

/* Delete the surface and texture.  The decoding thread must be stopped, and this
 * is normally done after destroying the decoder. */
void MovieTexture_Generic::DestroyTexture()
{
<<<<<<< HEAD
	delete surface_;
	surface_ = nullptr;

	delete texture_lock_;
	texture_lock_ = nullptr;
=======
	delete m_pSurface;
	m_pSurface = nullptr;

	delete m_pTextureLock;
	m_pTextureLock = nullptr;
>>>>>>> main

	if (texture_handle_)
	{
		DISPLAY->DeleteTexture(texture_handle_);
		texture_handle_ = 0;
	}

<<<<<<< HEAD
	intermediate_texture_ = nullptr;
=======
	delete m_pRenderTarget;
	m_pRenderTarget = nullptr;
	delete m_pTextureIntermediate;
	m_pTextureIntermediate = nullptr;
>>>>>>> main
}

class RageMovieTexture_Generic_Intermediate : public RageTexture
{
public:
	RageMovieTexture_Generic_Intermediate(RageTextureID ID, int iWidth, int iHeight,
		int iImageWidth, int iImageHeight, int iTextureWidth, int iTextureHeight,
		RageSurfaceFormat SurfaceFormat, RagePixelFormat pixfmt) :
		RageTexture(ID),
		m_SurfaceFormat(SurfaceFormat)
	{
		m_PixFmt = pixfmt;
		m_iSourceWidth = iWidth;
		m_iSourceHeight = iHeight;

		m_iImageWidth = iImageWidth;
		m_iImageHeight = iImageHeight;
		m_iTextureWidth = iTextureWidth;
		m_iTextureHeight = iTextureHeight;

		CreateFrameRects();

		texture_handle_ = 0;
		CreateTexture();
	}
	virtual ~RageMovieTexture_Generic_Intermediate()
	{
		if (texture_handle_)
		{
			DISPLAY->DeleteTexture(texture_handle_);
			texture_handle_ = 0;
		}
	}

	virtual void Invalidate() { texture_handle_ = 0; }
	virtual void Reload() { }
	virtual uintptr_t GetTexHandle() const
	{
		return texture_handle_;
	}

	bool IsAMovie() const { return true; }
private:
	void CreateTexture()
	{
		if (texture_handle_)
			return;

		RageSurface* pSurface = CreateSurfaceFrom(m_iImageWidth, m_iImageHeight,
			m_SurfaceFormat.BitsPerPixel,
			m_SurfaceFormat.Mask[0],
			m_SurfaceFormat.Mask[1],
			m_SurfaceFormat.Mask[2],
<<<<<<< HEAD
			m_SurfaceFormat.Mask[3], nullptr, 1);
=======
			m_SurfaceFormat.Mask[3], nullptr, 1 );
>>>>>>> main

		texture_handle_ = DISPLAY->CreateTexture(m_PixFmt, pSurface, false);
		delete pSurface;
	}

<<<<<<< HEAD
	uintptr_t texture_handle_;
=======
	uintptr_t m_uTexHandle;
>>>>>>> main
	RageSurfaceFormat m_SurfaceFormat;
	RagePixelFormat m_PixFmt;
};

void MovieTexture_Generic::Invalidate()
{
<<<<<<< HEAD
	texture_handle_ = 0;
	if (intermediate_texture_ != nullptr)
		intermediate_texture_->Invalidate();
=======
	m_uTexHandle = 0;
	if( m_pTextureIntermediate != nullptr )
		m_pTextureIntermediate->Invalidate();
>>>>>>> main
}

void MovieTexture_Generic::CreateTexture()
{
<<<<<<< HEAD
	if (texture_handle_ || render_target_ != nullptr)
=======
	if( m_uTexHandle || m_pRenderTarget != nullptr )
>>>>>>> main
		return;

	CHECKPOINT;

	m_iSourceWidth = decoder_->GetWidth();
	m_iSourceHeight = decoder_->GetHeight();

	/* Adjust m_iSourceWidth to support different source aspect ratios. */
	float fSourceAspectRatio = decoder_->GetSourceAspectRatio();
	if (fSourceAspectRatio < 1)
		m_iSourceHeight = std::lrint(m_iSourceHeight / fSourceAspectRatio);
	else if (fSourceAspectRatio > 1)
		m_iSourceWidth = std::lrint(m_iSourceWidth * fSourceAspectRatio);

	/* Cap the max texture size to the hardware max. */
	int iMaxSize = min( GetID().iMaxSize, DISPLAY->GetMaxTextureSize() );
	m_iImageWidth = min( m_iSourceWidth, iMaxSize );
	m_iImageHeight = min( m_iSourceHeight, iMaxSize );

	/* Texture dimensions need to be a power of two; jump to the next. */
	m_iTextureWidth = power_of_two(m_iImageWidth);
	m_iTextureHeight = power_of_two(m_iImageHeight);
	MovieDecoderPixelFormatYCbCr fmt = PixelFormatYCbCr_Invalid;
<<<<<<< HEAD
	if (surface_ == nullptr)
	{
		ASSERT(texture_lock_ == nullptr);
		if (g_bMovieTextureDirectUpdates)
			texture_lock_ = DISPLAY->CreateTextureLock();

		surface_ = decoder_->CreateCompatibleSurface(m_iImageWidth, m_iImageHeight,
			TEXTUREMAN->GetPrefs().m_iMovieColorDepth == 32, fmt);
		if (texture_lock_ != nullptr)
		{
			delete[] surface_->pixels;
			surface_->pixels = nullptr;
=======
	if( m_pSurface == nullptr )
	{
		ASSERT( m_pTextureLock == nullptr );
		if( g_bMovieTextureDirectUpdates )
			m_pTextureLock = DISPLAY->CreateTextureLock();

		m_pSurface = m_pDecoder->CreateCompatibleSurface( m_iImageWidth, m_iImageHeight,
			TEXTUREMAN->GetPrefs().m_iMovieColorDepth == 32, fmt );
		if( m_pTextureLock != nullptr )
		{
			delete [] m_pSurface->pixels;
			m_pSurface->pixels = nullptr;
>>>>>>> main
		}

	}

	RagePixelFormat pixfmt = DISPLAY->FindPixelFormat(surface_->format->BitsPerPixel,
		surface_->format->Mask[0],
		surface_->format->Mask[1],
		surface_->format->Mask[2],
		surface_->format->Mask[3]);

	if (pixfmt == RagePixelFormat_Invalid)
	{
		/* We weren't given a natively-supported pixel format.  Pick a supported
		 * one.  This is a fallback case, and implies a second conversion. */
		int depth = TEXTUREMAN->GetPrefs().m_iMovieColorDepth;
		switch (depth)
		{
		default:
			FAIL_M(ssprintf("Unsupported movie color depth: %i", depth));
		case 16:
			if (DISPLAY->SupportsTextureFormat(RagePixelFormat_RGB5))
				pixfmt = RagePixelFormat_RGB5;
			else
				pixfmt = RagePixelFormat_RGBA4;

			break;

		case 32:
			if (DISPLAY->SupportsTextureFormat(RagePixelFormat_RGB8))
				pixfmt = RagePixelFormat_RGB8;
			else if (DISPLAY->SupportsTextureFormat(RagePixelFormat_RGBA8))
				pixfmt = RagePixelFormat_RGBA8;
			else if (DISPLAY->SupportsTextureFormat(RagePixelFormat_RGB5))
				pixfmt = RagePixelFormat_RGB5;
			else
				pixfmt = RagePixelFormat_RGBA4;
			break;
		}
	}

	if (fmt != PixelFormatYCbCr_Invalid)
	{
		intermediate_texture_.reset();
		sprite_->UnloadTexture();

		/* Create the render target.  This will receive the final, converted texture. */
		RenderTargetParam param;
		param.iWidth = m_iImageWidth;
		param.iHeight = m_iImageHeight;

		RageTextureID TargetID(GetID());
		TargetID.filename += " target";
		render_target_ = std::make_unique<RageTextureRenderTarget>(TargetID, param);

		/* Create the intermediate texture.  This receives the YUV image. */
		RageTextureID IntermedID(GetID());
		IntermedID.filename += " intermediate";

		intermediate_texture_ = std::make_unique<RageMovieTexture_Generic_Intermediate>(IntermedID,
			decoder_->GetWidth(), decoder_->GetHeight(),
			surface_->w, surface_->h,
			power_of_two(surface_->w), power_of_two(surface_->h),
			*surface_->format, pixfmt);

		/* Configure the sprite.  This blits the intermediate onto the ifnal render target. */
		sprite_->SetHorizAlign(align_left);
		sprite_->SetVertAlign(align_top);

		/* Hack: Sprite wants to take ownership of the texture, and will decrement the refcount
		 * when it unloads the texture.  Normally we'd make a "copy", but we can't access
		 * RageTextureManager from here.  Just increment the refcount. */
		++intermediate_texture_->m_iRefCount;
		sprite_->SetTexture(intermediate_texture_.get());
		sprite_->SetEffectMode(GetEffectMode(fmt));

		return;
	}

<<<<<<< HEAD
	texture_handle_ = DISPLAY->CreateTexture(pixfmt, surface_, false);
=======
	m_uTexHandle = DISPLAY->CreateTexture( pixfmt, m_pSurface, false );
}

/* Handle decoding for a frame.  Return true if a frame was decoded, false if not
 * (due to quit, error, EOF, etc).  If true is returned, we'll be in FRAME_DECODED. */
bool MovieTexture_Generic::DecodeFrame()
{
	bool bTriedRewind = false;
	do
	{
		if( m_bWantRewind )
		{
			if( bTriedRewind )
			{
				LOG->Trace( "File \"%s\" looped more than once in one frame", GetID().filename.c_str() );
				return false;
			}
			m_bWantRewind = false;
			bTriedRewind = true;

			/* When resetting the clock, set it back by the length of the last frame,
			 * so it has a proper delay. */
			float fDelay = m_pDecoder->GetFrameDuration();

			/* Restart. */
			m_pDecoder->Rewind();

			m_fClock = -fDelay;
		}

		CHECKPOINT;

		/* Read a frame. */
		float fTargetTime = -1;
		if( m_bFrameSkipMode && m_fClock > m_pDecoder->GetTimestamp() )
			fTargetTime = m_fClock;

		int ret = m_pDecoder->DecodeFrame( fTargetTime );
		if( ret == -1 )
			return false;

		if( m_bWantRewind && m_pDecoder->GetTimestamp() == 0 )
			m_bWantRewind = false; /* ignore */

		if( ret == 0 )
		{
			/* EOF. */
			if( !m_bLoop )
				return false;

			LOG->Trace( "File \"%s\" looping", GetID().filename.c_str() );
			m_bWantRewind = true;
			continue;
		}

		/* We got a frame. */
	} while( m_bWantRewind );

	return true;
>>>>>>> main
}

/*
 * Returns:
 *  <= 0 if it's time for the next frame to display
 *   > 0 (seconds) if it's not yet time to display
 */
float MovieTexture_Generic::CheckFrameTime()
{
	if (rate_ == 0) {
		return 1;	// "a long time until the next frame"
	}
	return (decoder_->GetTimestamp() - clock_) / rate_;
}

void MovieTexture_Generic::UpdateMovie(float seconds)
{
<<<<<<< HEAD
	// Quick exit in case we failed to decode the movie.
	if (failure_) {
=======
	m_fClock += fSeconds * m_fRate;

	/* We might need to decode more than one frame per update.  However, there
	 * have been bugs in ffmpeg that cause it to not handle EOF properly, which
	 * could make this never return, so let's play it safe. */
	int iMax = 4;
	while( --iMax )
	{
		/* If we don't have a frame decoded, decode one. */
		if( m_ImageWaiting == FRAME_NONE )
		{
			if( !DecodeFrame() )
				break;

			m_ImageWaiting = FRAME_DECODED;
		}

		/* If we have a frame decoded, see if it's time to display it. */
		float fTime = CheckFrameTime();
		if( fTime > 0 )
			return;

		CHECKPOINT;

		UpdateFrame();

		m_ImageWaiting = FRAME_NONE;

>>>>>>> main
		return;
	}
	clock_ += seconds * rate_;

	// If the frame isn't ready, don't update. This does mean the video
	// will "speed up" to catch up when decoding does outpace display.
	//
	// In practice, display should rarely, if ever, outpace decoding.
	if (decoder_->IsCurrentFrameReady() && CheckFrameTime() <= 0) {
		UpdateFrame();
		return;
	}
}

void MovieTexture_Generic::UpdateFrame()
{
	if (finished_) {
		return;
	}

	/* Just in case we were invalidated: */
	CreateTexture();

<<<<<<< HEAD
	if (texture_lock_ != nullptr)
	{
		uintptr_t iHandle = intermediate_texture_ != nullptr ? intermediate_texture_->GetTexHandle() : this->GetTexHandle();
		texture_lock_->Lock(iHandle, surface_);
	}

	int frame_ret = decoder_->GetFrame(surface_);

	// Are we looping?
	if (decoder_->EndOfMovie() && loop_) {
		LOG->Info("File \"%s\" looping", GetID().filename.c_str());
		decoder_->Rollover();
		clock_ = 0.0;
	}
	else if (decoder_->EndOfMovie()) {
		// At the end of the movie, and not looping.
		finished_ = true;
	}

	// There's an issue with the frame, make sure it does not get
	// uploaded.
	if (frame_ret == -1) {
		if (texture_lock_ != nullptr) {
			texture_lock_->Unlock(surface_, true);
		}
		return;
=======
	if( m_pTextureLock != nullptr )
	{
<<<<<<< HEAD
		int iHandle = m_pTextureIntermediate != nullptr? m_pTextureIntermediate->GetTexHandle(): this->GetTexHandle();
=======
		uintptr_t iHandle = m_pTextureIntermediate != nullptr? m_pTextureIntermediate->GetTexHandle(): this->GetTexHandle();
>>>>>>> origin/unified-ui-features-13937230807013224518
		m_pTextureLock->Lock( iHandle, m_pSurface );
	}

	m_pDecoder->GetFrame( m_pSurface );
	if( m_pTextureLock != nullptr )
		m_pTextureLock->Unlock( m_pSurface, true );

	if( m_pRenderTarget != nullptr )
	{
		CHECKPOINT;

		/* If we have no m_pTextureLock, we still have to upload the texture. */
		if( m_pTextureLock == nullptr )
<<<<<<< HEAD
=======
		{
>>>>>>> origin/unified-ui-features-13937230807013224518
			DISPLAY->UpdateTexture(
				m_pTextureIntermediate->GetTexHandle(),
				m_pSurface,
				0, 0,
				m_pSurface->w, m_pSurface->h );
		CHECKPOINT;

		m_pRenderTarget->BeginRenderingTo( false );
		m_pSprite->Draw();
		m_pRenderTarget->FinishRenderingTo();
>>>>>>> main
	}

	if (texture_lock_ != nullptr) {
		texture_lock_->Unlock(surface_, true);
	}

	if (render_target_ != nullptr)
	{
<<<<<<< HEAD
		CHECKPOINT_M("About to upload the texture.");

		/* If we have no texture_lock_, we still have to upload the texture. */
		if (texture_lock_ == nullptr) {
=======
<<<<<<< HEAD
		CHECKPOINT;
		if( m_pTextureLock == nullptr )
=======
		if( m_pTextureLock == nullptr )
		{
>>>>>>> origin/unified-ui-features-13937230807013224518
>>>>>>> main
			DISPLAY->UpdateTexture(
				intermediate_texture_->GetTexHandle(),
				surface_,
				0, 0,
<<<<<<< HEAD
				surface_->w, surface_->h);
		}
		render_target_->BeginRenderingTo(false);
		sprite_->Draw();
		render_target_->FinishRenderingTo();
	}
	else {
		if (texture_lock_ == nullptr) {
			DISPLAY->UpdateTexture(
				texture_handle_,
				surface_,
				0, 0,
				m_iImageWidth, m_iImageHeight);
		}
=======
				m_iImageWidth, m_iImageHeight );
		CHECKPOINT;
>>>>>>> main
	}
}

static EffectMode EffectModes[] =
{
	EffectMode_YUYV422,
};
static_assert(ARRAYLEN(EffectModes) == NUM_PixelFormatYCbCr);

EffectMode MovieTexture_Generic::GetEffectMode(MovieDecoderPixelFormatYCbCr fmt)
{
	ASSERT(fmt != PixelFormatYCbCr_Invalid);
	return EffectModes[fmt];
}

void MovieTexture_Generic::Reload()
{
}

void MovieTexture_Generic::SetPosition(float seconds)
{
	// TODO: The only non-zero use case of this function would be practice mode.
	// Implement this by mathing out fSeconds and frame counts to seek the
	// video.
	if (seconds != 0)
	{
		LOG->Warn("MovieTexture_Generic::SetPosition(%f): non-0 seeking unsupported; ignored", seconds);
		return;
	}

	LOG->Trace("Seek to %f", seconds);
	clock_ = 0;
	decoder_->Rewind();
}

uintptr_t MovieTexture_Generic::GetTexHandle() const
{
<<<<<<< HEAD
	if (render_target_ != nullptr)
		return render_target_->GetTexHandle();
=======
	if( m_pRenderTarget != nullptr )
		return m_pRenderTarget->GetTexHandle();
>>>>>>> main

	return texture_handle_;
}

/*
 * (c) 2003-2005 Glenn Maynard
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
