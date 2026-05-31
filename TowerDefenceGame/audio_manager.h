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
		MIX_Track* track = MIX_CreateTrack(mixer);
		MIX_SetTrackAudio(track, audio);
		MIX_PlayAudio(mixer, audio);
	}

	void PlayFadeOutMusic(MIX_Audio* audio,int ms)
	{
		MIX_Track* track = MIX_CreateTrack(mixer);
		MIX_SetTrackAudio(track, audio);
		MIX_PlayTrack(track, NULL);
		MIX_StopTrack(track, MIX_TrackMSToFrames(track, ms));
	}

	void PlayBGM(MIX_Audio* audio, int fade_ms)
	{
		MIX_SetTrackAudio(bgm_track, audio);

		SDL_PropertiesID props = SDL_CreateProperties();
		SDL_SetNumberProperty(props, MIX_PROP_PLAY_FADE_IN_MILLISECONDS_NUMBER, fade_ms);
		SDL_SetNumberProperty(props, MIX_PROP_PLAY_LOOPS_NUMBER, -1);//循环播放

		MIX_PlayTrack(bgm_track, props);

		SDL_DestroyProperties(props);
	}

protected:
	AudioManager()
	{
		mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
		bgm_track = MIX_CreateTrack(mixer);
	}
	~AudioManager()
	{
		MIX_DestroyTrack(bgm_track);
		MIX_DestroyMixer(mixer);
	}

private:
	MIX_Mixer* mixer = nullptr;
	MIX_Track* bgm_track = nullptr;
};

#endif // !_AUDIO_MANAGER_H_
