#ifndef STREAM_MANAGER_H
#define STREAM_MANAGER_H

#include <string>
#include "RageThreads.h"

// Forward decls to avoid including ffmpeg headers in .h if possible,
// but we might need structs. For now, use void* opaque handles.
struct AVFormatContext;
struct AVCodecContext;
struct AVStream;
struct AVFrame;

class StreamManager
{
public:
	static StreamManager* Instance();
	static void Destroy();

	StreamManager();
	~StreamManager();

	bool StartStream( const std::string& rtmpUrl, int width, int height, int fps );
	void StopStream();
	bool IsStreaming() const;

	// Call this every frame with the backbuffer texture (stubbed as raw RGB)
	void PushFrame( void* rawPixels, int width, int height );

private:
	static StreamManager* s_pInstance;

	bool m_bStreaming;
	std::string m_Url;
	int m_Width;
	int m_Height;
	int m_FPS;

	// FFmpeg Contexts
	AVFormatContext* m_FormatCtx;
	AVCodecContext* m_CodecCtx;
	AVStream* m_Stream;
	AVFrame* m_Frame;

	RageMutex m_Mutex;
};

#endif
