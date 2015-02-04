
#include "BGLAudio.h"

std::unordered_map<std::string, Mix_Chunk *> BGLAudio::sounds;


bool BGLAudio::Initialize()
{
	Mix_Init(MIX_INIT_OGG | MIX_INIT_MP3);
	int32_t result = Mix_OpenAudio(PLAYBACK_FREQUENCY, AUDIO_FORMAT, MIXER_CHANNELS, AUDIO_BUFFER);
	Mix_AllocateChannels(16);
	return (result == 0);
}

void BGLAudio::LoadMusic(std::string tag, std::string path)
{

}

void BGLAudio::PlayMusic(std::string tag, int32_t loops)
{

}

void BGLAudio::LoadAudio(std::string tag, std::string path)
{
	Mix_Chunk *sound = 0;
	sound = Mix_LoadWAV(path.c_str());
	if(!sound)
	{
		std::cout << "WARNING: Could not load sound at: " << path << std::endl;
		return;
	}
	
	sounds[tag] = sound;
}

void BGLAudio::PlayAudio(std::string tag, int32_t loops)
{

	Mix_Chunk *s = sounds[tag];

	if(!s)
	{
		std::cout << "WARNING: Could not play the sound with tag: " << tag << std::endl;
		return;
	}

	int32_t channel = Mix_PlayChannel(-1, s, loops);
}

void BGLAudio::SetVolume(uint32_t volume)
{
	uint32_t v = SDL_max(volume, MIX_MAX_VOLUME);
	Mix_Volume(-1, v);
}


void BGLAudio::Cleanup()
{
	// TODO(brett): Before we use this function we need to make sure none of the channels are playing

	while(Mix_Playing(-1))
	{
		SDL_Delay(100);
	}

	for(auto i = sounds.begin(); i != sounds.end(); ++i)
	{
		if(i->second)
			Mix_FreeChunk(i->second);
	}

	Mix_CloseAudio();
}