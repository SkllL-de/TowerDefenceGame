#ifndef _CONFIG_MANAGER_H_
#define _CONFIG_MANAGER_H_

#include "map.h"
#include "manager.h"
#include <SDL3/SDL.h>
#include <cJSON.h>
#include <string>
#include <fstream>
#include <sstream>

class ConfigManager : public Manager<ConfigManager>
{
	friend class Manager<ConfigManager>;

public:
	struct BasicTemplate
	{
		std::string window_title = "村庄保卫战！";
		int window_width = 1280;
		int window_height = 720;
	};

	struct PlayerTemplate
	{
		double speed = 3;
		double normal_attack_interval = 0.5;
		double normal_attack_demage = 0;
		double skill_interval = 10;
		double skill_damage = 1;
	};

	struct TowerTemplate
	{
		double interval[10] = { 1 };
		double damage[10] = { 25 };
		double view_range[10] = { 5 };
		double cost[10] = { 50 };
		double upgrade_cost[9] = { 75 };
	};

	struct EnemyTemplate
	{
		double hp = 100;
		double speed = 1;
		double damage = 1;//进入保护点扣除的生命值
		double reward_ratio = 0.5;//爆金币的概率
		double recover_interval = 10;//释放范围性恢复技能的间隔
		double recover_range = 0;//释放范围性恢复技能的距离，为0时只治疗自己
		double recover_intensity = 25;//治疗值
	};

public:
	Map map;
protected:
	ConfigManager() = default;
	~ConfigManager() = default;


};

#endif // !_CONFIG_MANAGER_H_
