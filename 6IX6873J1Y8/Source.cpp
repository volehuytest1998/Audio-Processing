#include <SDL.h>

#define FILE_PATH "intro.wav"

struct AudioData
{
	Uint8* pos;
	Uint32 length;
};

// Play device. This function not return anything.
void MyAudioCallBack(void* userData, Uint8* stream, int streamLength)
{
	AudioData* audio = (AudioData*)userData;

	if (audio->length == 0)
	{
		// nothing to play
		return;
	}

	// get length to play
	Uint32 length = ((Uint32)streamLength > audio->length ? audio->length : (Uint32)streamLength);
	// put to stream
	SDL_memcpy(stream, audio->pos, length);

	audio->pos += length;
	audio->length -= length;
}

int main(int, char**)
{
	if (SDL_Init(SDL_INIT_AUDIO) != 0)
	{
		// init audio system fail
		SDL_Log("Unable to initialize SDL_INIT_AUDIO: %s", SDL_GetError());
	}
	else
	{
		SDL_AudioSpec wavSpec;

		Uint8* wavStart;
		Uint32 wavLength;

		if (SDL_LoadWAV(FILE_PATH, &wavSpec, &wavStart, &wavLength) == NULL)
		{
			// load file wav fail
			SDL_Log("Unable to load file wav: %s", SDL_GetError());
		}
		else
		{
			// load file wave success

			AudioData audio;
			audio.pos = wavStart;
			audio.length = wavLength;

			wavSpec.callback = MyAudioCallBack;
			wavSpec.userdata = &audio;

			// zero is playback
			SDL_AudioDeviceID device = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, SDL_AUDIO_ALLOW_ANY_CHANGE);
			if (device == 0)
			{
				// open audio device error
				SDL_Log("Unable to open audio device: %s", SDL_GetError());
			}
			else
			{
				SDL_PauseAudioDevice(device, 0);
				while (audio.length > 0)
				{
					SDL_Delay(100);
				}
			}

			SDL_CloseAudioDevice(device);
			SDL_FreeWAV(wavStart);
		}
	}

	SDL_Quit();
	return 0;
}