#include <windows.h>
#include <mmsystem.h>

// Helper function to fill a buffer with silence
void FillBufferWithSilence(LPSTR buffer, DWORD bufferSize)
{
    for (DWORD i = 0; i < bufferSize; i++)
    {
        buffer[i] = 0;
    }
}

int main()
{
    // Open the default waveform-audio output device
    HWAVEOUT hWaveOut = NULL;
    waveOutOpen(&hWaveOut, WAVE_MAPPER, NULL, 0, 0, 0);

    // Set up wave format structure
    WAVEFORMATEX waveFormat;
    waveFormat.wFormatTag = WAVE_FORMAT_PCM;
    waveFormat.nChannels = 1;
    waveFormat.nSamplesPerSec = 44100;
    waveFormat.nAvgBytesPerSec = 44100;
    waveFormat.nBlockAlign = 1;
    waveFormat.wBitsPerSample = 8;
    waveFormat.cbSize = 0;

    // Set up wave header structure
    WAVEHDR waveHeader;
    waveHeader.lpData = new char[44100];
    waveHeader.dwBufferLength = 44100;
    waveHeader.dwBytesRecorded = 0;
    waveHeader.dwUser = 0;
    waveHeader.dwFlags = 0;
    waveHeader.dwLoops = 0;
    waveHeader.lpNext = 0;
    waveHeader.reserved = 0;

    // Fill the buffer with silence
    FillBufferWithSilence(waveHeader.lpData, waveHeader.dwBufferLength);

    // Prepare and write the wave header
    waveOutPrepareHeader(hWaveOut, &waveHeader, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &waveHeader, sizeof(WAVEHDR));

    // Wait for the sound to finish playing
    Sleep(1000);

    // Clean up
    waveOutUnprepareHeader(hWaveOut, &waveHeader, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
    delete[] waveHeader.lpData;

    return 0;
}
