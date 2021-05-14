#ifndef __AUDIO_H__
#define __AUDIO_H__

#include "Module.h"

#include "Point.h"
#include "List.h"

#define MAX_CHANNELS			360.0f
#define MAX_FX					100.0f

#define MAX_DISTANCE			500.0f
#define MAX_DISTANCE_VOL		250.0f

#define MUTE_DISTANCE			510.0f
#define MUTE_DISTANCE_VOL		255.0f

#define RADS_TO_DEG				180 / 3.14

#define DEFAULT_MUSIC_FADE_TIME 1.0f
struct _Mix_Music;
struct Mix_Chunk;

enum SoundTrack
{
	NO_TRACK = 0,
	MAINMENU_TRACK,
	OPTIONSMENU_TRACK,
	MAINVILLAGE_TRACK,
	MAINCOMBAT_TRACK,
	LEVELUP_TRACK,
	ENDSCENE_TRACK,
	GRASSYLANDS_TRACK
};

enum Effect
{
	NO_FX = -1,
	BUTTON_FOCUSSED,
	BUTTON_RELEASED,
	LOGO_SCENE_FX,
	MAIN_MENU_FX,
	PLAYER_TURN_FX,
	PLAYER_ATTACK_FX,
	PLAYER_HURT_FX,
	WOLF_TURN_FX,
	WOLF_ATTACK_FX,
	WOLF_HURT_FX,
	BIRD_TURN_FX,
	MANTIS_TURN_FX,
	MANTIS_ATTACK_FX,
	MANTIS_HURT_FX
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

	void LoadAllFx(pugi::xml_node& fx_node);
	unsigned int LoadFx(const char* path);
	void SetFx(Effect fx);

	//Tools
	uint GetAngle(iPoint player, iPoint enemy);
	uint GetVolumeFromDistance(iPoint player, iPoint enemy);
	void SetChannelAngles();

	SoundTrack GetPlayingMusic() const
	{
		return st;
	}

	int GetMusicVolume();
	int GetFxVolume();

	void TransitionVolumeMusic();
	void ChangeVolumeMusic(int volume);
	void ChangeVolumeFx(int volume);
	void TogglePauseMusic();
	void StopMusic();
	int ValueToVolume(int value, int maxPercent = 100);
	int VolumeToValue(int volume, int maxPercent = 100);

private:

	bool PlayMusic(const char* path, float fadeTime = DEFAULT_MUSIC_FADE_TIME);

	bool PlayFx(unsigned int fx, int repeat = 0);
	bool PlayFxOnChannel(uint id, uint channel, uint distance = 1, int repeat = 0);

private:

	const iPoint yAxis = { 0, 1 };

	_Mix_Music* music;
	List<Mix_Chunk *> fx;
};

#endif // __AUDIO_H__