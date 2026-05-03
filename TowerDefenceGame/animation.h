#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include "timer.h"

#include <SDL3/SDL.h>
#include <vector>
#include <functional>

class Animaiton
{
public:
	typedef std::function<void()> PlayCallback;
public:
	Animaiton()
	{
		timer.set_one_shot(false);
		timer.set_on_timeout(
			[&]()
			{
				idx_frame++;
				if (idx_frame >= rect_src_list.size())
				{
					idx_frame = is_loop ? 0 : rect_src_list.size() - 1;
					if (!is_loop && on_finished)
						on_finished();
				}
			}
		);
	}

	~Animaiton() = default;

	void reset()
	{
		timer.restart();

		idx_frame = 0;
	}

	void set_frame_data(SDL_Texture* texture, int num_h, int num_v, std::vector<int>& idx_list)
	{
		float width_tex, height_tex;

		this->texture = texture;
		SDL_GetTextureSize(texture, &width_tex, &height_tex);
		width_frame = width_tex / num_h, height_frame = height_tex / num_v;

		rect_src_list.resize(idx_list.size());
		for (size_t i = 0; i < idx_list.size(); i++)
		{
			int idx = idx_list[i];
			SDL_FRect& rect_src = rect_src_list[i];

			rect_src.x = (idx % num_h) * width_frame;
			rect_src.y = (idx / num_v) * height_frame;
			rect_src.w = width_tex, rect_src.h = height_tex;
		}
	}

	void set_loop(bool is_loop)
	{
		this->is_loop = is_loop;
	}

	void set_interval(double interval)
	{
		timer.set_wait_time(interval);
	}

	void set_on_finished(PlayCallback on_finished)
	{
		this->on_finished = on_finished;
	}

	void on_update(double delta)
	{
		timer.on_update(delta);
	}

	void on_render(SDL_Renderer* renderer, const SDL_Point& pos_dst, double angle = 0) const
	{
		static SDL_FRect rect_dst;

		rect_dst.x = pos_dst.x, rect_dst.y = pos_dst.y;
		rect_dst.w = width_frame, rect_dst.h = height_frame;

		SDL_RenderTexture(renderer, texture, &rect_src_list[idx_frame], &rect_dst);
	}

private:
	Timer timer;
	bool is_loop = true;//动画是否循环播放
	size_t idx_frame = 0;//索引：用于找到数组中对应"动画的帧"的"图片裁剪矩形"
	PlayCallback on_finished;
	SDL_Texture* texture = nullptr;
	std::vector<SDL_FRect> rect_src_list;
	int width_frame = 0, height_frame = 0;

};


#endif // !_ANIMATION_H_
