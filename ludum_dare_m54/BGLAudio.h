#pragma once

#include "language.h"

#include <unordered_map>
#include <SDL.h>
#include <SDL_mixer.h>
#include <string>

#define PLAYBACK_FREQUENCY	22050
#define AUDIO_FORMAT		MIX_DEFAULT_FORMAT
#define MIXER_CHANNELS		6
#define AUDIO_BUFFER		4096

class BGLAudio
{
public:
	static bool Initialize();
	static void LoadAudio(std::string tag, std::string path);
	static void PlayAudio(std::string tag, int32_t loops = 0);
	static void SetVolume(uint32_t volume);

	static void Cleanup();
private:
	static std::unordered_map<std::string, Mix_Chunk *> sounds;
};
