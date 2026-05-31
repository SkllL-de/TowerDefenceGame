#ifndef  _TOWER_H_
#define _TOWER_H_

#include "facing.h"
#include "vector2.h"
#include "animation.h"
#include "tower_type.h"
#include "bullet_manager.h"
#include "enemy_manager.h"
#include "audio_manager.h"

class Tower
{
public:
	Tower()
	{
		timer_fire.set_one_shot(true);
		timer_fire.set_on_timeout(
			[&]()
			{
				can_fire = true;
			});

		anim_idle_down.set_loop(true);
		anim_idle_down.set_interval(0.2);
		anim_idle_up.set_loop(true);
		anim_idle_up.set_interval(0.2);
		anim_idle_left.set_loop(true);
		anim_idle_left.set_interval(0.2);
		anim_idle_right.set_loop(true);
		anim_idle_right.set_interval(0.2);

		anim_fire_up.set_loop(false);
		anim_fire_up.set_interval(0.2);
		anim_fire_up.set_on_finished(
			[&]()
			{
				update_idle_animation();
			});

		anim_fire_down.set_loop(false);
		anim_fire_down.set_interval(0.2);
		anim_fire_down.set_on_finished(
			[&]()
			{
				update_idle_animation();
			});

		anim_fire_left.set_loop(false);
		anim_fire_left.set_interval(0.2);
		anim_fire_left.set_on_finished(
			[&]()
			{
				update_idle_animation();
			});

		anim_fire_right.set_loop(false);
		anim_fire_right.set_interval(0.2);
		anim_fire_right.set_on_finished(
			[&]()
			{
				update_idle_animation();
			});

	}
	~Tower() = default;

	void set_position(const Vector2& position)
	{
		this->position = position;
	}

	const Vector2& get_size() const
	{
		return size;
	}

	const Vector2& get_position() const
	{
		return position;
	}
	void on_update(double delta)
	{
		timer_fire.on_update(delta);
		anim_currrent->on_update(delta);

		if (can_fire) on_fire();
	}

	void on_render(SDL_Renderer* renderer)
	{
		static SDL_Point point;

		point.x = (int)(position.x - size.x / 2);
		point.y = (int)(position.y - size.y / 2);

		anim_currrent->on_render(renderer, point);
	}

protected:
	Vector2 size;

	Animation anim_idle_up;
	Animation anim_idle_down;
	Animation anim_idle_left;
	Animation anim_idle_right;
	Animation anim_fire_up;
	Animation anim_fire_down;
	Animation anim_fire_left; 
	Animation anim_fire_right;

	TowerType tower_type = TowerType::Archer;

	double fire_speed = 0;
	BulletType bullet_type = BulletType::Arrow;

private:
	Timer timer_fire;
	Vector2 position;
	bool can_fire = false;
	Facing facing = Facing::Down;
	Animation* anim_currrent = &anim_idle_down;

private:
	void update_idle_animation()
	{
		switch (facing)
		{
		case Facing::Up:
			anim_currrent = &anim_idle_up;
			break;
		case Facing::Down:
			anim_currrent = &anim_idle_down;
			break;
		case Facing::Left:
			anim_currrent = &anim_idle_left;
			break;
		case Facing::Right:
			anim_currrent = &anim_idle_right;
			break;
		default:
			break;
		}
	}

	void update_fire_animation()
	{
		switch (facing)
		{
		case Facing::Up:
			anim_currrent = &anim_fire_up;
			break;
		case Facing::Down:
			anim_currrent = &anim_fire_down;
			break;
		case Facing::Left:
			anim_currrent = &anim_fire_left;
			break;
		case Facing::Right:
			anim_currrent = &anim_fire_right;
			break;
		default:
			break;
		}
	}
	
	Enemy* find_target_enemy()
	{
		double process = -1;
		double view_range = 0;
		Enemy* enemy_target = nullptr;
		
		static ConfigManager* instance = ConfigManager::instance();

		switch (tower_type)
		{
		case Archer:
			view_range = instance->archer_template.view_range[instance->level_archer];
			break;
		case Axeman:
			view_range = instance->axeman_template.view_range[instance->level_axeman];
			break;
		case Gunner:
			view_range = instance->gunner_template.view_range[instance->level_gunner];
			break;
		}

		EnemyManager::EnemyList& enemy_list = EnemyManager::instance()->get_enemy_list();

		for (Enemy* enemy : enemy_list)
		{
			if ((enemy->get_position() - position).length() <= view_range * SIZE_TILE)
			{
				double new_process = enemy->get_route_process();
				if (new_process > process)
				{
					enemy_target = enemy;
					process = new_process;
				}
			}
		}
		return enemy_target;
	}

	void on_fire()
	{
		Enemy* target_enemy = find_target_enemy();

		if (!target_enemy) return;

		can_fire = false;
		static ConfigManager* instance = ConfigManager::instance();
		static const ResourcesManager::AudioPool& audio_pool = ResourcesManager::instance()->get_audio_pool();
		static AudioManager* audio_manager = AudioManager::instance();

		double interval = 0, damage = 0;

		switch (tower_type)
		{
		case Archer:
			interval = instance->archer_template.interval[instance->level_archer];
			damage = instance->archer_template.damage[instance->level_archer];
			switch (rand() % 2)
			{
			case 0:
				audio_manager->PlayMusic(audio_pool.find(ResID::Sound_ArrowFire_1)->second);
				break;
			case 1:
				audio_manager->PlayMusic(audio_pool.find(ResID::Sound_ArrowFire_2)->second);
				break;
			}
			break;
		case Axeman:
			interval = instance->axeman_template.interval[instance->level_axeman];
			damage = instance->axeman_template.damage[instance->level_axeman];
			audio_manager->PlayMusic(audio_pool.find(ResID::Sound_Axe_Fire)->second);
			break;
		case Gunner:
			interval = instance->gunner_template.interval[instance->level_gunner];
			damage = instance->gunner_template.damage[instance->level_gunner];
			audio_manager->PlayMusic(audio_pool.find(ResID::Sound_Shell_Fire)->second);
			break;
		}
		timer_fire.set_wait_time(interval);
		timer_fire.restart();

		Vector2 direction = target_enemy->get_position() - position;
		BulletManager::instance()->fire_bullet(bullet_type, position, fire_speed, direction.normalize() * fire_speed * SIZE_TILE, damage, target_enemy);

		bool is_show_x_anim = abs(direction.x) >= abs(direction.y);
		if (is_show_x_anim)
			facing = direction.x > 0 ? Facing::Right : Facing::Right;
		else
			facing = direction.y > 0 ? Facing::Down : Facing::Up;

		update_fire_animation();
		anim_currrent->reset();
	}
};

#endif // ! _TOWER_H_
