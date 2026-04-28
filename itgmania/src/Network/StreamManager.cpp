#include "global.h"
#include "StreamManager.h"
#include "RageLog.h"

// FFmpeg includes
extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/imgutils.h>
#include <libavutil/opt.h>
}

StreamManager* StreamManager::s_pInstance = NULL;

StreamManager* StreamManager::Instance()
{
	if( !s_pInstance )
		s_pInstance = new StreamManager;
	return s_pInstance;
}

void StreamManager::Destroy()
{
	delete s_pInstance;
	s_pInstance = NULL;
}

StreamManager::StreamManager() : m_Mutex("StreamManager")
{
	m_bStreaming = false;
	m_FormatCtx = NULL;
	m_CodecCtx = NULL;
	m_Stream = NULL;
	m_Frame = NULL;

	// Initialize FFmpeg (required for older versions like 0.15/2.x)
	av_register_all();
	avformat_network_init();
}

StreamManager::~StreamManager()
{
	StopStream();
}

bool StreamManager::StartStream( const std::string& rtmpUrl, int width, int height, int fps )
{
	LockMut(m_Mutex);
	if( m_bStreaming ) return false;

	LOG->Info( "StreamManager: Initializing Stream to %s (%dx%d @ %d FPS)", rtmpUrl.c_str(), width, height, fps );

	m_Url = rtmpUrl;
	m_Width = width;
	m_Height = height;
	m_FPS = fps;

	// 1. Output Context (FLV for RTMP)
	int ret = avformat_alloc_output_context2( &m_FormatCtx, NULL, "flv", m_Url.c_str() );
	if( !m_FormatCtx )
	{
		LOG->Warn( "StreamManager: Could not create output context." );
		return false;
	}

	// 2. Add Video Stream (H.264)
	AVCodec* codec = avcodec_find_encoder( AV_CODEC_ID_H264 );
	if( !codec )
	{
		LOG->Warn( "StreamManager: H.264 Encoder not found." );
		return false;
	}

	m_Stream = avformat_new_stream( m_FormatCtx, codec );
	if( !m_Stream ) return false;

	m_CodecCtx = m_Stream->codec;
	m_CodecCtx->codec_id = AV_CODEC_ID_H264;
	m_CodecCtx->width = m_Width;
	m_CodecCtx->height = m_Height;
	m_Stream->time_base = (AVRational){1, m_FPS};
	m_CodecCtx->time_base = m_Stream->time_base;
	m_CodecCtx->pix_fmt = AV_PIX_FMT_YUV420P; // Standard for streaming

	// Set H.264 presets (ultrafast for low latency)
	av_opt_set( m_CodecCtx->priv_data, "preset", "ultrafast", 0 );
	av_opt_set( m_CodecCtx->priv_data, "tune", "zerolatency", 0 );

	// 3. Open Codec
	if( avcodec_open2( m_CodecCtx, codec, NULL ) < 0 )
	{
		LOG->Warn( "StreamManager: Failed to open codec." );
		return false;
	}

	// 4. Open Network
	if( !(m_FormatCtx->oformat->flags & AVFMT_NOFILE) )
	{
		if( avio_open( &m_FormatCtx->pb, m_Url.c_str(), AVIO_FLAG_WRITE ) < 0 )
		{
			LOG->Warn( "StreamManager: Failed to open URL %s", m_Url.c_str() );
			return false;
		}
	}

	// 5. Write Header
	if( avformat_write_header( m_FormatCtx, NULL ) < 0 )
	{
		LOG->Warn( "StreamManager: Failed to write header." );
		return false;
	}

	// 6. Allocate Frame
	m_Frame = av_frame_alloc();
	m_Frame->format = m_CodecCtx->pix_fmt;
	m_Frame->width = m_CodecCtx->width;
	m_Frame->height = m_CodecCtx->height;
	av_image_alloc( m_Frame->data, m_Frame->linesize, m_Width, m_Height, m_CodecCtx->pix_fmt, 32 );

	m_bStreaming = true;
	LOG->Info( "StreamManager: Streaming Started!" );
	return true;
}

void StreamManager::StopStream()
{
	LockMut(m_Mutex);
	if( !m_bStreaming ) return;

	LOG->Info( "StreamManager: Stopping Stream..." );

	if( m_FormatCtx )
	{
		av_write_trailer( m_FormatCtx );
		if( !(m_FormatCtx->oformat->flags & AVFMT_NOFILE) )
			avio_closep( &m_FormatCtx->pb );
		avformat_free_context( m_FormatCtx );
	}

	if( m_Frame )
	{
		av_freep( &m_Frame->data[0] );
		av_frame_free( &m_Frame );
	}

	m_FormatCtx = NULL;
	m_CodecCtx = NULL;
	m_Stream = NULL;
	m_Frame = NULL;

	m_bStreaming = false;
}

bool StreamManager::IsStreaming() const
{
	return m_bStreaming;
}

void StreamManager::PushFrame( void* rawPixels, int width, int height )
{
	LockMut(m_Mutex);
	if( !m_bStreaming ) return;

	// In a real implementation:
	// 1. Convert raw RGB (OpenGL) to YUV420P (sws_scale).
	// 2. m_Frame->data = yuv_pixels;
	// 3. m_Frame->pts = frame_count++;
	// 4. avcodec_encode_video2(...)
	// 5. av_interleaved_write_frame(...)
}
