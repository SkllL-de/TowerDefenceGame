#ifndef _PAUSE_BANNER_H_
#define _PAUSE_BANNER_H_


#include "config_manager.h"
#include "resources_manager.h"

#include <SDL3/SDL.h>
#include <SDL3_gfxPrimitives.h>
class PauseBanner
{
public:
	PauseBanner()
	{
		window_width = ConfigManager::instance()->basic_template.window_width;
		window_height = ConfigManager::instance()->basic_template.window_height;

		size_text = { 645, 215 };

		pos_center = { (double)window_width / 2, (double)window_height / 2 };
	
	}
	~PauseBanner() = default;

	void on_update()
	{
		tex_text = ResourcesManager::instance()->get_texture_pool().find(ResID::Tex_UIPauseText)->second;
	}

	void on_render(SDL_Renderer* renderer)
	{
		//SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		//SDL_SetRenderDrawColor(renderer, 128, 128, 128, 128);

		//SDL_GetRenderOutputSize(renderer, &window_width, &window_height);
		//static SDL_FRect rect_dst = { 0, 0, window_width, window_height	};

		//SDL_RenderFillRect(renderer, &rect_dst);

		boxRGBA(renderer, 0, 0, window_width, window_height, 38, 40, 44, 160);

		static SDL_FRect rect_dst;

		rect_dst.x = (pos_center.x - size_text.x / 2);
		rect_dst.y = (pos_center.y - size_text.y / 2);
		rect_dst.w = size_text.x, rect_dst.h = size_text.y;
		SDL_RenderTexture(renderer, tex_text, nullptr, &rect_dst);

	}
private:
	int window_width = 1280;
	int window_height = 960;

	Vector2 pos_center;
	Vector2 size_text;

	SDL_Texture* tex_text = nullptr;
};

#endif // !_PAUSE_BANNER_H_
