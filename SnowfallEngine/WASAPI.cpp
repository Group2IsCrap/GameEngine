#include "WASAPI.h"

//Handle Audio Stream
HRESULT PlayAudioStream(snowFallAudio::WASAPIAudio::AudioSource* audioSource)
{
	HRESULT hr;
	REFERENCE_TIME requestedDuration = REFTIMES_PER_SEC;
	REFERENCE_TIME actualDuration;
	IMMDeviceEnumerator* enumerator = NULL;
	IMMDevice* device = NULL;
	IAudioClient* audioClient = NULL;
	IAudioRenderClient* renderClient = NULL;
	WAVEFORMATEX* pwfx = NULL;
	UINT32 bufferFrameCount;
	UINT32 numFramesAvailable;
	UINT32 numFramesPadding;
	BYTE* data;
	DWORD flags = 0;

	// Create Instance
	hr = CoCreateInstance(
		CLSID_MMDeviceEnumerator, NULL, CLSCTX_ALL, IID_IMMDeviceEnumerator, (void**)&enumerator);
	EXIT_ON_ERROR(hr)

	// Get the default audio device
		hr = enumerator->GetDefaultAudioEndpoint(eRender, eConsole, &device);
	EXIT_ON_ERROR(hr)

	// Activate the retrieved device with an audio client
		hr = device->Activate(IID_IAudioClient, CLSCTX_ALL, NULL, (void**)&audioClient);
	EXIT_ON_ERROR(hr)

	// Get the format of that client
		hr = audioClient->GetMixFormat(&pwfx);
	EXIT_ON_ERROR(hr)

	// Initialize the client with the correct format and in shared mode so as to not break windows
		hr = audioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, 0, requestedDuration, 0, pwfx, NULL);
	EXIT_ON_ERROR(hr)

	// Determine the format to use
		hr = audioSource->SetFormat(pwfx);
	EXIT_ON_ERROR(hr)

	// Get the actual size of the allocated buffer
		hr = audioClient->GetBufferSize(&bufferFrameCount);
	EXIT_ON_ERROR(hr)

		hr = audioClient->GetService(IID_IAudioRenderClient, (void**)&renderClient);
	EXIT_ON_ERROR(hr)

	// Grab the entire buffer for initial fill
		hr = renderClient->GetBuffer(bufferFrameCount, &data);
	EXIT_ON_ERROR(hr)

	// Load the initial data to the shared buffer
		hr = audioSource->LoadData(bufferFrameCount, data, &flags);
	EXIT_ON_ERROR(hr)

		hr = renderClient->ReleaseBuffer(bufferFrameCount, flags);
	EXIT_ON_ERROR(hr)

	// Get the actual duration of the allocated buffer
	actualDuration = (double)REFTIMES_PER_SEC * bufferFrameCount / pwfx->nSamplesPerSec;

	// Start playing
		hr = audioClient->Start();
	EXIT_ON_ERROR(hr)

	while (flags != AUDCLNT_BUFFERFLAGS_SILENT)
	{
		// Sleep for half duration
		Sleep((DWORD)(actualDuration / REFTIMES_PER_MILLISEC / 2));

		// Find out how much buffer space is available
		hr = audioClient->GetCurrentPadding(&numFramesPadding);
		EXIT_ON_ERROR(hr)

		numFramesAvailable = bufferFrameCount - numFramesPadding;

		// Grab all available space
		hr = renderClient->GetBuffer(numFramesAvailable, &data);
		EXIT_ON_ERROR(hr)

		// Get next half second of data from source
		hr = audioSource->LoadData(numFramesAvailable, data, &flags);
		EXIT_ON_ERROR(hr)

		hr = renderClient->ReleaseBuffer(numFramesAvailable, flags);
		EXIT_ON_ERROR(hr)
	}

	// Wait till end of buffer before stopping to stop overlap
	Sleep((DWORD)(actualDuration / REFTIMES_PER_MILLISEC / 2));

	hr = audioClient->Stop();
	EXIT_ON_ERROR(hr)

	Exit:
		CoTaskMemFree(pwfx);
		SAFE_RELEASE(enumerator)
			SAFE_RELEASE(device)
			SAFE_RELEASE(audioClient)
			SAFE_RELEASE(renderClient)

			return hr;
}