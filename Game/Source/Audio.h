#ifndef __AUDIO_H__
#define __AUDIO_H__

#include "Module.h"

#include "List.h"

#define DEFAULT_MUSIC_FADE_TIME 1.0f

struct _Mix_Music;
struct Mix_Chunk;

enum SoundTrack
{
	NO_TRACK = 0,
	MAINMENU_TRACK,
	MAINVILLAGE_TRACK,
	MAINCOMBAT_TRACK,
};

enum Effect
{
	NO_FX = -1,
	BUTTON_FOCUSSED,
	BUTTON_RELEASED,
	LOGO_SCENE_FX,
	MAIN_MENU_FX
};

class AudioManager : public Module
{
public:

	SoundTrack st = NO_TRACK;

	AudioManager();

	virtual ~AudioManager();

	bool Awake(pugi::xml_node&);

	bool CleanUp();

	void SetMusic(SoundTrack sc, float fadeTime = DEFAULT_MUSIC_FADE_TIME);

	void SetFx(Effect fx);

	unsigned int LoadFx(const char* path);

	SoundTrack GetPlayingMusic() const
	{
		return st;
	}

	void ChangeVolumeMusic();
	void TogglePauseMusic();
	void StopMusic();

private:
	bool PlayMusic(const char* path, float fadeTime = DEFAULT_MUSIC_FADE_TIME);

	bool PlayFx(unsigned int fx, int repeat = 0);

	_Mix_Music* music;
	List<Mix_Chunk *> fx;
};

#endif // __AUDIO_H__