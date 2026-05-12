#ifndef _WAVE_MANAGER_H_
#define _WAVE_MANAGER_H_

#include "timer.h"
#include "manager.h"
#include "config_manager.h"

class WaveManager : public Manager<WaveManager>
{
	friend class Manager<WaveManager>;

public:

protected:
	WaveManager()
	{
		static const std::vector<Wave>& wave_list = ConfigManager::instance()->wave_list;

		timer_start_wave.set_one_shot(true);
		timer_start_wave.set_wait_time(wave_list[0].interval);
		timer_start_wave.set_on_timeout(
			[&]()
			{
				is_wave_started = true;
				timer_spawn_enemy.set_wait_time(wave_list[idx_wave].spwan_event_list[0].interval);
				timer_spawn_enemy.restart();
			}
		);

		timer_spawn_enemy.set_one_shot(true);
		timer_spawn_enemy.set_on_timeout(
			[&]()
			{

			}
		);

	}
	~WaveManager(){}

private:
	int idx_wave = 0;//当前是第几波
	int idx_spawn_event = 0;//当前波次的敌人中，轮到第几个
	Timer timer_start_wave;//游戏开始一段时间后，才开始生成怪物
	Timer timer_spawn_enemy;
	bool is_wave_started = false;
	bool is_spwan_last_enemy = false;

};

#endif // !_WAVE_MANAGER_H_
