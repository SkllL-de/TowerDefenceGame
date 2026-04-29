#ifndef _GAME_MANAGER_H_
#define _GAME_MANAGER_H_

#include "manager.h"
#include "config_manager.h"
#include "resources_manager.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_gfxPrimitives.h>

class GameManager : public Manager<GameManager>
{
	friend class Manager<GameManager>;
public:
	int run(int argc, char** argv)
	{
		Uint64 last_counter = SDL_GetPerformanceCounter();
		Uint64 counter_freq = SDL_GetPerformanceFrequency();

		while (!is_quit)
		{
			while (SDL_PollEvent(&event))
			{
				on_input();
			}
			Uint64 current_counter = SDL_GetPerformanceCounter();
			double delta = static_cast<double>(current_counter - last_counter) / (counter_freq);
			if (delta * 1000 < 1000.0 / 60)
			{
				SDL_Delay(static_cast<Uint32>(1000.0 / 60 - delta * 1000));
			}
			on_update(delta);
			
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderClear(renderer);

			on_render();

			SDL_RenderPresent(renderer);
		}
		return 0;
	}
protected:
	GameManager()
	{
		init_assert(SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO), "SDL3初始化失败！");
		init_assert(MIX_Init(), "SDL_mixer初始化失败！");
		init_assert(TTF_Init(), "SDL_ttf初始化失败！");

		MIX_Mixer* mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);

		SDL_SetHint(SDL_HINT_IME_IMPLEMENTED_UI, "0");//使用系统默认的输入法界面

		window = SDL_CreateWindow("村庄保卫战", 1280, 720, 0);
		init_assert(window, "创建游戏窗口失败！");

		SDL_PropertiesID props = SDL_CreateProperties();
		SDL_SetPointerProperty(props, SDL_PROP_RENDERER_CREATE_WINDOW_POINTER, window);//将窗口指针设置到属性中，供渲染器创建时使用
		SDL_SetNumberProperty(props, SDL_PROP_RENDERER_CREATE_PRESENT_VSYNC_NUMBER, 1);//启用垂直同步
		renderer = SDL_CreateRendererWithProperties(props);
		init_assert(renderer, "创建渲染器失败！");
	}
	~GameManager()
	{
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);

		TTF_Quit();
		MIX_Quit();
		SDL_Quit();
	}
	//GameManager() = default;
	//~GameManager() = default;
	GameManager(const GameManager&) = delete;
	GameManager& operator=(const GameManager&) = delete;

private:
	SDL_Event event;
	bool is_quit = false;

	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
private:
	void on_input()
	{

	}
	void on_update(double delta)
	{

	}
	void on_render()
	{
	}
	void init_assert(bool flag, const char* err_msg)
	{
		if (flag) return;

		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "游戏启动失败", err_msg, window);
		exit(-1);
	}
};
#endif // !_GAME_MANAGER_H_
