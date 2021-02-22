#ifndef __AUDIO_H__
#define __AUDIO_H__

#include "Module.h"

#include "List.h"

#define DEFAULT_MUSIC_FADE_TIME 2.0f

struct _Mix_Music;
struct Mix_Chunk;

class AudioManager : public Module
{
public:

	AudioManager();

	virtual ~AudioManager();

	bool Awake(pugi::xml_node&);

	bool CleanUp();

	bool PlayMusic(const char* path, float fadeTime = DEFAULT_MUSIC_FADE_TIME);

	unsigned int LoadFx(const char* path);

	bool PlayFx(unsigned int fx, int repeat = 0);

private:

	_Mix_Music* music;
	List<Mix_Chunk *> fx;
};

#endif // __AUDIO_H__