#pragma once

#include "language.h"


#include <Windows.h>

#include <unordered_map>
#include <SDL.h>
#include <SDL_mixer.h>
#include <string>

#define PLAYBACK_FREQUENCY	22050 // 44100 
#define AUDIO_FORMAT		MIX_DEFAULT_FORMAT
#define MIXER_CHANNELS		6
#define AUDIO_BUFFER		1024

// This is the audio handler. SDL2 Mixer does not work as well as I would like it to.
// Perhaps I can switch it out for FMOD
class BGLAudio
{
public:
	static bool Initialize();
	static void LoadAudio(std::string tag, std::string path);
	static void PlayAudio(std::string tag, int32_t loops = 0);

	void LoadMusic(std::string tag, std::string path);
	void PlayMusic(std::string tag, int32_t loops);

	static void SetVolume(uint32_t volume);

	static void Cleanup();
private:
	static std::unordered_map<std::string, Mix_Chunk *> sounds;
};
