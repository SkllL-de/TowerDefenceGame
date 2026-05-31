#ifndef _BULLET_H_
#define _BULLET_H_

#include "vector2.h"
#include "enemy.h"
#include "animation.h"
#include "config_manager.h"

class Bullet
{
public:
	Bullet() = default;
	~Bullet() = default;

	void set_velocity(const Vector2& velocity)
	{
		this->velocity = velocity;

		if (can_rotated)
		{
			double radian = std::atan2(velocity.y, velocity.x);
			angle_anim_rotated = radian * 180 / 3.1415926;
		}
	}

	void set_position(const Vector2& position)
	{
		this->position = position;
	}

	void set_damage(double damage)
	{
		this->damage = damage;
	}

	void set_speed(double speed)
	{
		this->speed = speed;
	}

	void set_target_enemy(Enemy* target_enemy)
	{
		this->target_enemy = target_enemy;
	}

	const Vector2& get_size() const
	{
		return size;
	}

	const Vector2& get_position() const
	{
		return position;
	}

	double get_speed()
	{
		return speed;
	}

	double get_damage() const
	{
		return damage;
	}

	double get_damage_range() const
	{
		return damage_range;
	}

	void disable_collide()
	{
		is_collisional = false;
	}

	bool can_collide() const
	{
		return is_collisional;
	}

	void make_invalid()
	{
		is_valid = false;
		is_collisional = false;
	}

	bool can_remove() const
	{
		return !is_valid;
	}

	Enemy* get_target_enemy() const
	{
		return target_enemy;
	}

	virtual void on_update(double delta)
	{
		if (!target_enemy || target_enemy->can_remove()) 
		{
			is_valid = false;
			return;
		}
		
		animation.on_update(delta);

		Vector2 direction = target_enemy->get_position() - position;
		set_velocity(direction.normalize() * speed * SIZE_TILE);

		static const SDL_FRect& rect_map
			= ConfigManager::instance()->rect_tile_map;
		
		position += velocity * delta;

		if (position.x - size.x / 2 <= rect_map.x
			|| position.x + size.x / 2 >= rect_map.x + rect_map.w
			|| position.y - size.y / 2 <= rect_map.y
			|| position.y + size.y / 2 >= rect_map.y + rect_map.h)
		{
			is_valid = false;
		}
	}

	virtual void on_render(SDL_Renderer* renderer)
	{
		static SDL_Point point;

		point.x = (int)(position.x - size.x / 2);
		point.y = (int)(position.y - size.y / 2);

		animation.on_render(renderer, point, angle_anim_rotated);
	}

	virtual void on_collide(Enemy* enemy)
	{
		if (enemy != target_enemy) return;
		is_valid = false;
		is_collisional = false;
	}

protected:
	Vector2 size;
	Vector2 velocity;
	Vector2 position;
	
	Animation animation;
	bool can_rotated = false;

	double damage = 0;
	double damage_range = -1;

private:
	bool is_valid = true;
	bool is_collisional = true;
	double angle_anim_rotated = 0;
	double speed = 0;
	Enemy* target_enemy = nullptr;
};


#endif // !_BULLET_H_
