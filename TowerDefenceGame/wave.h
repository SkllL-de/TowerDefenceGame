#ifndef _WAVE_H_
#define _WAVE_H_

#include "enemy_type.h"

#include <vector>

struct Wave
{
	struct SpwanEvent
	{
		double interval = 0;//波次内敌人生成间隔
		int spwan_point = 1;
		EnemyType enemy_type = EnemyType::Slim;
	};

	double rewards = 0;//每波结束的奖励
	double interval = 0;//波次之间的间隔
	std::vector<SpwanEvent> spwan_event_list;
};

#endif // !_WAVE_H_
