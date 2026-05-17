#ifndef _AUDIO_MANAGER_H_
#define _AUDIO_MANAGER_H_

#include "manager.h"

#include <SDL3_mixer/SDL_mixer.h>

class AudioManager : public Manager<AudioManager>
{
	friend class Manager<AudioManager>;
public:
	void PlayMusic(MIX_Audio* audio)
	{
		MIX_PlayAudio(mixer, audio);
	}

	void PlayBGM(MIX_Audio* audio)
	{
		MIX_Track* track = MIX_CreateTrack(mixer);
		MIX_SetTrackAudio(track, audio);

		SDL_PropertiesID props = SDL_CreateProperties();
		SDL_SetNumberProperty(props, MIX_PROP_PLAY_LOOPS_NUMBER, -1);//循环播放

		MIX_PlayTrack(track, props);

		SDL_DestroyProperties(props);
	}

protected:
	AudioManager()
	{
		mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
	}
	~AudioManager()
	{
		MIX_DestroyMixer(mixer);
	}

private:
	MIX_Mixer* mixer = nullptr;
};

#endif // !_AUDIO_MANAGER_H_
