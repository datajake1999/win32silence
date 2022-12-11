#include <windows.h>
#include <mmsystem.h>

#define samplerate 44100

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCommandLine, int cmdShow)
{
	WAVEFORMATEX waveFormat;
	WORD samples[samplerate];
	WAVEHDR waveHeader;
	HWAVEOUT hWaveOut = NULL;
	HANDLE hMutex = NULL;

	//Attempt to create the mutex and return if already open
	hMutex = OpenMutex(MUTEX_ALL_ACCESS, 0, "Win32Silence");
	if (hMutex)
	{
		return 0;
	}
	hMutex = CreateMutex(0, 0, "Win32Silence");

	// Set up wave format structure
	ZeroMemory(&waveFormat, sizeof(waveFormat));
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nChannels = 1;
	waveFormat.nSamplesPerSec = samplerate;
	waveFormat.nAvgBytesPerSec = samplerate*2;
	waveFormat.nBlockAlign = 2;
	waveFormat.wBitsPerSample = 16;

	// Set up audio buffer
	ZeroMemory(samples, sizeof(samples));

	// Set up wave header structure
	ZeroMemory(&waveHeader, sizeof(waveHeader));
	waveHeader.lpData = samples;
	waveHeader.dwBufferLength = sizeof(samples);

	// Attempt to open the default waveform-audio output device
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &waveFormat, 0, 0, 0);
	if (!hWaveOut)
	{
		return 0;
	}

	// Prepare the wave header
	waveOutPrepareHeader(hWaveOut, &waveHeader, sizeof(WAVEHDR));

	while(1)
	{
		// Write audio to device
		waveOutWrite(hWaveOut, &waveHeader, sizeof(WAVEHDR));

		// Wait for the sound to finish playing
		Sleep(1000);
	}

	// Clean up
	waveOutReset(hWaveOut);
	waveOutUnprepareHeader(hWaveOut, &waveHeader, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
	hWaveOut = NULL;
	ReleaseMutex(hMutex);
	hMutex = NULL;

	return 0;
}
