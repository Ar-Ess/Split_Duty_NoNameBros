#include "App.h"
#include "Audio.h"
#include "Textures.h"

#include "Defs.h"
#include "Log.h"

#include "SDL/include/SDL.h"
#include "SDL_mixer/include/SDL_mixer.h"

AudioManager::AudioManager() : Module()
{
	music = NULL;
	name.Create("audio");
}

AudioManager::~AudioManager()
{}

bool AudioManager::Awake(pugi::xml_node& config)
{
	LOG("Loading Audio Mixer");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
	{
		LOG("SDL_INIT_AUDIO could not initialize! SDL_Error: %s\n", SDL_GetError());
		active = false;
		ret = true;
	}

	// Load support for the OGG formats
	int flags = MIX_INIT_OGG;
	int init = Mix_Init(flags);

	if((init & flags) != flags)
	{
		LOG("Could not initialize Mixer lib. Mix_Init: %s", Mix_GetError());
		active = false;
		ret = true;
	}

	// Initialize SDL_mixer
	if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		LOG("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		active = false;
		ret = true;
	}

	//LOADING ALL SFX
	pugi::xml_node sFx = config.child("fx");
	LoadAllFx(sFx);

	return ret;
}

bool AudioManager::CleanUp()
{
	if(!active)
		return true;

	LOG("Freeing sound FX, closing Mixer and Audio subsystem");

	if(music != NULL)
	{
		Mix_FreeMusic(music);
	}

	ListItem<Mix_Chunk*>* item;
	for(item = fx.start; item != NULL; item = item->next)
		Mix_FreeChunk(item->data);

	fx.Clear();

	Mix_CloseAudio();
	Mix_Quit();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);

	return true;
}

void AudioManager::SetMusic(SoundTrack sc, float fadeTime)
{
	if (sc == NO_TRACK)
	{
		return; //STOP MUSIC
	}
	else if (sc == MAINMENU_TRACK)
	{
		if (st != sc) PlayMusic("Assets/Audio/Music/SplitDuty_MainMenu_Soundtrack.mp3", fadeTime);
	}
	else if (sc == MAINVILLAGE_TRACK)
	{
		if (st != sc) PlayMusic("Assets/Audio/Music/SplitDuty_MainVillage_Soundtrack.mp3", fadeTime);
	}
	else if (sc == MAINCOMBAT_TRACK)
	{
		if (st != sc) PlayMusic("Assets/Audio/Music/SplitDuty_MainCombat_Soundtrack.mp3", fadeTime);
	}

	st = sc;
}

void AudioManager::SetFx(Effect fx)
{
	if (fx == BUTTON_FOCUSSED)
	{
		PlayFx(1);
	}
	else if (fx == BUTTON_RELEASED)
	{
		PlayFx(2);
	}
	else if (fx == LOGO_SCENE_FX)
	{
		PlayFx(3);
	}
	else if (fx == MAIN_MENU_FX)
	{
		PlayFx(4);
	}
}

void AudioManager::ChangeVolumeMusic()
{
	if (Mix_VolumeMusic(-1) == MIX_MAX_VOLUME)
		Mix_VolumeMusic(MIX_MAX_VOLUME / 3);
	else
		Mix_VolumeMusic(MIX_MAX_VOLUME);		
}

void AudioManager::TogglePauseMusic()
{
	if (Mix_PausedMusic)
		Mix_PauseMusic();
	else
		Mix_ResumeMusic();
}

void AudioManager::StopMusic()
{
	Mix_HaltMusic();
}

bool AudioManager::PlayMusic(const char* path, float fadeTime)
{
	bool ret = true;

	if(!active)
		return false;

	if(music != NULL)
	{
		if(fadeTime > 0.0f)
		{
			Mix_FadeOutMusic(int(fadeTime * 1000.0f));
		}
		else
		{
			Mix_HaltMusic();
		}

		// this call blocks until fade out is done
		Mix_FreeMusic(music);
	}

	music = Mix_LoadMUS(path);

	if(music == NULL)
	{
		LOG("Cannot load music %s. Mix_GetError(): %s\n", path, Mix_GetError());
		ret = false;
	}
	else
	{
		if(fadeTime > 0.0f)
		{
			if(Mix_FadeInMusic(music, -1, (int) (fadeTime * 1000.0f)) < 0)
			{
				LOG("Cannot fade in music %s. Mix_GetError(): %s", path, Mix_GetError());
				ret = false;
			}
		}
		else
		{
			if(Mix_PlayMusic(music, -1) < 0)
			{
				LOG("Cannot play in music %s. Mix_GetError(): %s", path, Mix_GetError());
				ret = false;
			}
		}
	}

	LOG("Successfully playing %s", path);
	return ret;
}

void AudioManager::LoadAllFx(pugi::xml_node& fx_node)
{
	for (pugi::xml_node sound = fx_node.child("sound");
		sound != nullptr; sound = sound.next_sibling("sound"))
	{
		const char* path = sound.child_value();
		LoadFx(path);
	}
}

unsigned int AudioManager::LoadFx(const char* path)
{
	unsigned int ret = 0;

	if(!active)
		return 0;

	Mix_Chunk* chunk = Mix_LoadWAV(path);

	if(chunk == NULL)
	{
		LOG("Cannot load wav %s. Mix_GetError(): %s", path, Mix_GetError());
	}
	else
	{
		fx.Add(chunk);
		ret = fx.Count();
	}

	return ret;
}

bool AudioManager::PlayFx(unsigned int id, int repeat)
{
	bool ret = false;

	if(!active)
		return false;

	if(id > 0 && id <= fx.Count())
	{
		Mix_VolumeChunk(fx[id - 1], 40);
		Mix_PlayChannel(-1, fx[id - 1], repeat);
	}

	return ret;
}
