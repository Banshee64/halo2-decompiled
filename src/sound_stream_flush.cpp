// @flags /O2 /arch:SSE /Gr /Ob1
/* SOUND_STREAM_FLUSH.CPP: flushing a streamed sound (moved out of
   src/unknown_2ae170.cpp: retail calls it out of line from 0x12a5d0 and
   0x2ae750, and /Ob1 keeps LTCG from inlining it) */

#include "cseries.h"
#include <xtl.h>
#include "unknown_2ae170.h"

// @retail 0x2ae4b0
void sound_stream_flush(s_sound_stream *stream)
{
	if (stream->state != 0)
	{
		DWORD status;
		stream->stream->GetStatus(&status);
		if (status & DSSTREAMSTATUS_PAUSED)
		{
			IDirectSoundStream_Pause(stream->stream, DSSTREAMPAUSE_RESUME);
		}
		IDirectSoundStream_FlushEx(stream->stream, 0, DSSTREAMFLUSHEX_ASYNC | DSSTREAMFLUSHEX_ENVELOPE);
		stream->codec->stop();
		stream->flushing = 1;
	}
}
