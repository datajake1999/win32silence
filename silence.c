#include <windows.h>
#include <mmsystem.h>

#define samplerate 44100

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCommandLine, int cmdShow)
{
	// Set up wave format structure
	WAVEFORMATEX waveFormat;
	ZeroMemory(&waveFormat, sizeof(waveFormat));
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nChannels = 1;
	waveFormat.nSamplesPerSec = samplerate;
	waveFormat.nAvgBytesPerSec = samplerate*2;
	waveFormat.nBlockAlign = 2;
	waveFormat.wBitsPerSample = 16;

	// Set up audio buffer
	WORD samples[samplerate];
	ZeroMemory(samples, sizeof(samples));

	// Set up wave header structure
	WAVEHDR waveHeader;
	ZeroMemory(&waveHeader, sizeof(waveHeader));
	waveHeader.lpData = samples;
	waveHeader.dwBufferLength = sizeof(samples);

	// Open the default waveform-audio output device
	HWAVEOUT hWaveOut = NULL;
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &waveFormat, 0, 0, 0);

	while(1)
	{
		// Prepare and write the wave header
		waveOutPrepareHeader(hWaveOut, &waveHeader, sizeof(WAVEHDR));
		waveOutWrite(hWaveOut, &waveHeader, sizeof(WAVEHDR));

		// Wait for the sound to finish playing
		Sleep(1000);

		// Unprepare the wave header
		waveOutUnprepareHeader(hWaveOut, &waveHeader, sizeof(WAVEHDR));
	}

	// Clean up
	waveOutClose(hWaveOut);
	hWaveOut = NULL;

	return 0;
}
