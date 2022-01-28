#pragma once

#include "Vector2D.h"
#include <Audioclient.h>
#include <Audiopolicy.h>
#include <mmdeviceapi.h>
#include <iostream>
#include <math.h>

#define REFTIMES_PER_SEC 10000000
#define REFTIMES_PER_MILLISEC 10000

#define EXIT_ON_ERROR(hres) \
			if(FAILED(hres)){goto Exit;}
#define SAFE_RELEASE(punk) \
			if((punk) != NULL) \
				{(punk)->Release(); (punk) = NULL;}

namespace snowFallAudio
{
	namespace WASAPIAudio
	{



		//Define IID values to save typing them all out
		const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
		const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
		const IID IID_IAudioClient = __uuidof(IAudioClient);
		const IID IID_IAudioRenderClient = __uuidof(IAudioRenderClient);

		//End of IID value definitions

		class AudioSource
		{
		public:
			HITRESULT LoadData(UINT32 bufferFrameCount, BYTE* data, DWORD* flags);
			HITRESULT SetFormat(WAVEFORMATEX* format);
		};
	}
}

