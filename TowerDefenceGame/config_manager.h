#ifndef _CONFIG_MANAGER_H_
#define _CONFIG_MANAGER_H_

#include "map.h"
#include "wave.h"
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
	std::vector<Wave> wave_list;

	int level_archer = 0;
	int level_axeman = 0;
	int level_gunner = 0;

	bool is_game_win = true;//gameover时房子生命值>0
	bool is_game_over = false;//所有波次已经结束,所有敌人已经被消灭
	SDL_Rect rect_tile_map = { 0 };

	BasicTemplate basic_template;

	PlayerTemplate player_template;

	TowerTemplate archer_template;
	TowerTemplate axeman_template;
	TowerTemplate gunner_template;

	EnemyTemplate slim_template;
	EnemyTemplate king_slim_template;
	EnemyTemplate skeleton_template;
	EnemyTemplate goblin_template;
	EnemyTemplate goblin_priest_template;

	const double num_initial_hp = 10;//初始目标生命值
	const double num_initial_coin = 100;//初始金币数
	const double num_coin_per_prop = 10;//每次捡到金币道具时，增加的金币数

public:
	bool load_level_config(const std::string& path)
	{
		std::ifstream file(path);

		if (!file.good()) return false;//返回false点1：文件错误

		std::stringstream str_stream;
		str_stream << file.rdbuf(); file.close();

		cJSON* json_root = cJSON_Parse(str_stream.str().c_str());
		if (!json_root) return false;//返回false点2：根json解析错误

		if (json_root->type != cJSON_Array)//确保json_root是数组类型，否则短路退出
		{
			cJSON_Delete(json_root);
			return false;//返回false点3：json_root不是数组类型
		}

		cJSON* json_wave = nullptr;
		cJSON_ArrayForEach(json_wave, json_root)//for循环宏
		{
			if (json_wave->type != cJSON_Object)
				continue;

			wave_list.emplace_back();
			Wave& wave = wave_list.back();

			cJSON* json_wave_rewards = cJSON_GetObjectItem(json_wave, "rewards");
			if (json_wave_rewards && json_wave_rewards->type == cJSON_Number)
				wave.rewards = json_wave_rewards->valuedouble;

			cJSON* json_wave_interval = cJSON_GetObjectItem(json_wave, "interval");
			if (json_wave_interval && json_wave_interval->type == cJSON_Number)
				wave.interval = json_wave_interval->valuedouble;

			cJSON* json_wave_spwan_list = cJSON_GetObjectItem(json_wave, "spawn_list");
			if(json_wave_spwan_list && json_wave_spwan_list->type == cJSON_Array)
			{
				cJSON* json_spwan_event = nullptr;
				cJSON_ArrayForEach(json_spwan_event, json_wave_spwan_list)
				{
					if (json_spwan_event->type != cJSON_Object)
						continue;

					wave.spwan_event_list.emplace_back();
					Wave::SpwanEvent& spwan_event = wave.spwan_event_list.back();

					cJSON* json_spwan_event_interval = cJSON_GetObjectItem(json_spwan_event, "interval");
					if (json_spwan_event_interval && json_spwan_event_interval->type == cJSON_Number)
						spwan_event.interval = json_spwan_event_interval->valuedouble;

					cJSON* json_spwan_event_point = cJSON_GetObjectItem(json_spwan_event, "point");
					if (json_spwan_event_point && json_spwan_event_point->type == cJSON_Number)
						spwan_event.spwan_point = json_spwan_event_point->valueint;

					cJSON* json_spwan_event_enemy_type = cJSON_GetObjectItem(json_spwan_event, "enemty");
					if (json_spwan_event_enemy_type && json_spwan_event_enemy_type->type == cJSON_String)
					{
						const std::string str_enemy_type = json_spwan_event_enemy_type->valuestring;
						if (str_enemy_type == "Slim")
							spwan_event.enemy_type = EnemyType::Slim;
						else if (str_enemy_type == "KingSlim")
							spwan_event.enemy_type = EnemyType::KingSlim;
						else if (str_enemy_type == "Skeleton")
							spwan_event.enemy_type = EnemyType::Skeleton;
						else if (str_enemy_type == "Goblin")
							spwan_event.enemy_type = EnemyType::Goblin;
						else if (str_enemy_type == "GoblinPriest")
							spwan_event.enemy_type = EnemyType::GoblinPriest;
						//enemy_type不是以上任何一种类型时，enemy_type仍保留默认值Slim
					}
				}
				//如果控制添加spwan_event_list的cJSON_ForEach循环后，spwan_event仍为空，则删除在wave_list数组末尾新增的wave格
				if (wave.spwan_event_list.empty())
					wave_list.pop_back();
			}
		}
		cJSON_Delete(json_root);

		if (wave_list.empty())
			return false;//返回false点4：wave_list为空

		return true;
	}

	bool load_game_config(const std::string& path)
	{

	}
protected:
	ConfigManager() = default;
	~ConfigManager() = default;


};

#endif // !_CONFIG_MANAGER_H_
