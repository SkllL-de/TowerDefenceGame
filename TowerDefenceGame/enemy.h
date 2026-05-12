#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "timer.h"
#include "route.h"
#include "vector2.h"
#include "animation.h"
#include "config_manager.h"

#include <functional>

class Enemy
{
public:
	typedef std::function<void(Enemy* enemy)> SkillCallback;
public:
	Enemy()
	{
		timer_skill.set_one_shot(false);//敌人技能按照固定时间间隔触发
		timer_skill.set_on_timeout([&]() { on_skill_released(this); });//把"本对象"的"技能释放"传给计时器

		timer_sketch.set_one_shot(true);
		timer_sketch.set_wait_time(0.075);
		timer_sketch.set_on_timeout([&]() { is_show_sketch = false; });

		timer_restore_speed.set_one_shot(true);
		timer_restore_speed.set_on_timeout([&]() {speed = max_speed; });
	}
	~Enemy() = default;

	void on_update(double delta)
	{
		timer_skill.on_update(delta);
		timer_sketch.on_update(delta);
		timer_restore_speed.on_update(delta);

		Vector2 move_distance = velocity * delta;//当前这一帧(按照移动速度最长可以)移动的距离
		Vector2 target_distance = position_target - position;//实际移动到目标点,需要的距离
		position += move_distance < target_distance ? move_distance : target_distance;//保证移动后不越过目标点

		if (target_distance.approx_zero())
		{
			idx_target++;
			refresh_position_target();

			direction = (position_target - position).normalize();
		}

		velocity.x = direction.x * speed * SIZE_TILE;
		velocity.y = direction.y * speed * SIZE_TILE;	
		//velocity = direction * speed * SIZE_TILE;

		bool is_show_x_amina = abs(velocity.x) >= abs(velocity.y);

		if (is_show_sketch)
		{
			if (is_show_x_amina)
				anim_curent = velocity.x > 0 ? &anim_right_sketch : &anim_left_sketch;
			else
				anim_curent = velocity.y > 0 ? &anim_down_sketch : &anim_up_sketch;
		}
		else
		{
			if (is_show_x_amina)
				anim_curent = velocity.x > 0 ? &anim_right : &anim_left;
			else
				anim_curent = velocity.y > 0 ? &anim_down : &anim_up;
		}

		anim_curent->on_update(delta);
	}

	void on_render(SDL_Renderer* renderer)
	{
		static SDL_FRect rect;
		static SDL_Point point;
		static const float offset_y = 2;
		static const Vector2 size_hp_bar = { 40, 8 };
		static const SDL_Color color_border = { 116, 185, 124, 255 };//敌人血条边框(深绿)
		static const SDL_Color color_content = { 226, 255, 194, 255 };//敌人血条内容物(浅绿)

		point.x = position.x - size.x / 2;
		point.y = position.y - size.y / 2;

		anim_curent->on_render(renderer, point);

		if (hp < max_hp)
		{
			rect.x = position.x - size_hp_bar.x / 2;
			rect.y = position.y - size.y / 2 - size_hp_bar.y - offset_y;
			rect.w = size_hp_bar.x * (hp / max_hp);
			rect.h = size_hp_bar.y;
			SDL_SetRenderDrawColor(renderer,color_content.r,color_content.g,color_content.b,color_content.a);
			SDL_RenderFillRect(renderer, &rect);

			rect.w = size_hp_bar.x;
			SDL_SetRenderDrawColor(renderer, color_border.r, color_border.g, color_border.b, color_border.a);
			SDL_RenderFillRect(renderer, &rect);
		}
	}

	void set_on_skill_released(SkillCallback on_skill_released)
	{
		this->on_skill_released = on_skill_released;
	}

	void increase_hp(double val)
	{
		hp += val;

		if (hp > max_hp)
			hp = max_hp;
	}

	void decrease_hp(double val)
	{
		hp -= val;
		if (hp < 0)
		{
			hp = 0;
			is_valid = false;
		}

		is_show_sketch = true;
		timer_sketch.restart();
	}

	void slow_down()
	{
		speed = max_speed - 0.5;
		timer_restore_speed.set_wait_time(1);
		timer_restore_speed.restart();
	}

	void set_position(const Vector2& position)//在生成敌人时使用
	{
		this->position = position;
	}

	void set_route(const Route* route)
	{
		this->route = route;

		refresh_position_target();
	}

	void make_invalid()
	{
		is_valid = false;
	}

	double get_hp() const
	{
		return hp;
	}

	const Vector2& get_size() const
	{
		return size;
	}

	const Vector2& get_position() const
	{
		return position;
	}

	const Vector2& get_velocity() const
	{
		return velocity;
	}

	double get_damage() const
	{
		return damage;
	}

	double get_reward_ratio() const
	{
		return reward_ratio;
	}

	double get_recover_radius() const
	{
		return SIZE_TILE * recover_range;
	}
	double get_recover_intensity() const
	{
		return recover_intensity;
	}

	bool can_remove() const
	{
		return !is_valid;
	}

	double get_orute_process() const
	{
		if (route->get_idx_list().size() == 1)
			return 1;

		return (double)idx_target / (route->get_idx_list().size() - 1);
	}

protected:
	Vector2 size;//敌人大小

	Timer timer_skill;//敌人技能定时器

	Animaiton anim_up;
	Animaiton anim_down;
	Animaiton anim_left;
	Animaiton anim_right;
	Animaiton anim_up_sketch;
	Animaiton anim_down_sketch;
	Animaiton anim_left_sketch;
	Animaiton anim_right_sketch;

	double hp = 0;//实际血量
	double max_hp = 0;//初始血量（最大血量）
	double speed = 0;//实际速度
	double max_speed = 0;//初始速度（最大速度）
	double damage = 0;
	double reward_ratio = 0;
	double recover_interval = 0;
	double recover_range = 0;
	double recover_intensity = 0;

private:
	Vector2 position;//当前所在(窗口上的)位置
	Vector2 velocity;//当前移动速度
	Vector2 direction;//当前移动方向

	bool is_valid = true;//true:未被击杀, false:被击杀

	Timer timer_sketch;//负责控制修改is_show_sketchs,受击->闪(烁)白(光)
	bool is_show_sketch = false;//是否展示受击描边动画

	Animaiton* anim_curent = nullptr;//记录当前在画(上,下,左,右)哪一个动画

	SkillCallback on_skill_released;

	Timer timer_restore_speed;//减速效果恢复时间

	const Route* route = nullptr;//用于获取对应SDL_Point数组
	int idx_target = 0;//对应路径SDL_Point数组各个单元格的索引
	Vector2 position_target;//默认初始x=0,y=0

private:
	void refresh_position_target()
	{
		const Route::IdxList& idx_list = route->get_idx_list();

		if (idx_target < idx_list.size())
		{
			const SDL_Point& point = idx_list[idx_target];
			static const SDL_FRect& rect_tile_map = ConfigManager::instance()->rect_tile_map;

			position_target.x = rect_tile_map.x + point.x * SIZE_TILE + SIZE_TILE / 2;//下一个要走的目标点的位置.x = 地图左上角.x + 当前坐标点.x * SIZE_TILE + SIZE_TILE/2（到中心点）
			position_target.y = rect_tile_map.y + point.y * SIZE_TILE + SIZE_TILE / 2;//下一个要走的目标点的位置.x = 地图左上角.y + 当前坐标点.y * SIZE_TILE
		}
	}
};


#endif // !_ENEMY_H_
