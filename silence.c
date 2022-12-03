#include <stdlib.h>
#include <windows.h>
#include <mmsystem.h>

#define samplerate 44100

// Helper function to fill a buffer with silence
void FillBufferWithSilence(LPSTR buffer, DWORD bufferSize)
{
    for (DWORD i = 0; i < bufferSize; i++)
    {
        buffer[i] = 0x80;
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCommandLine, int cmdShow)
{
    // Set up wave format structure
    WAVEFORMATEX waveFormat;
    waveFormat.wFormatTag = WAVE_FORMAT_PCM;
    waveFormat.nChannels = 1;
    waveFormat.nSamplesPerSec = samplerate;
    waveFormat.nAvgBytesPerSec = samplerate;
    waveFormat.nBlockAlign = 1;
    waveFormat.wBitsPerSample = 8;
    waveFormat.cbSize = 0;

    // Set up wave header structure
    WAVEHDR waveHeader;
    waveHeader.lpData = (char *)malloc(samplerate);
    waveHeader.dwBufferLength = samplerate;
    waveHeader.dwBytesRecorded = 0;
    waveHeader.dwUser = 0;
    waveHeader.dwFlags = 0;
    waveHeader.dwLoops = 0;
    waveHeader.lpNext = 0;
    waveHeader.reserved = 0;

    // Fill the buffer with silence
    FillBufferWithSilence(waveHeader.lpData, waveHeader.dwBufferLength);

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
    }

    // Clean up
    waveOutUnprepareHeader(hWaveOut, &waveHeader, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
    free(waveHeader.lpData);

    return 0;
}
