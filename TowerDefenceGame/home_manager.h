#ifndef _HOMEMANAGER_H_
#define _HOMEMANAGER_H_

#include "manager.h"
#include "config_manager.h"
#include "resources_manager.h"
#include "audio_manager.h"


class HomeManager : public Manager<HomeManager>
{
	friend class Manager<HomeManager>;

public:
	double get_current_hp_num()
	{
		return num_hp;
	}

	void decrease_hp(double val)
	{
		num_hp -= val;

		if (num_hp <= 0)
		{
			num_hp = 0;
			ConfigManager::instance()->is_game_win = false;
			ConfigManager::instance()->is_game_over = true;
		}

		static const ResourcesManager::AudioPool& audio_pool
			= ResourcesManager::instance()->get_audio_pool();

		//MIX_PlayAudio(nullptr, audio_pool.find(ResID::Sound_HomeHurt)->second);

		AudioManager::instance()->PlayMusic(audio_pool.find(ResID::Sound_HomeHurt)->second);
	}

protected:
	HomeManager()
	{
		num_hp = ConfigManager::instance()->num_initial_hp;
	}
	~HomeManager() = default;

private:
	double num_hp = 0;
};

#endif // !_HOMEMANAGER_H_
