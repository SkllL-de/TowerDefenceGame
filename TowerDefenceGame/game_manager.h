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

		mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);

		SDL_SetHint(SDL_HINT_IME_IMPLEMENTED_UI, "0");//使用系统默认的输入法界面

		ConfigManager* config = ConfigManager::instance();

		init_assert(config->map.load("map.csv"), "加载游戏地图失败");//初始化了config_manager的Map map
		init_assert(config->load_level_config("level.json"), "加载关卡配置失败");//初始化了config_manager的vector<Wave> wave_list
		init_assert(config->load_game_config("config.json"), "加载游戏配置失败");//初始化了config_manager的所有Struct XxxxxTemplate

		window = SDL_CreateWindow(config->basic_template.window_title.c_str(), config->basic_template.window_width, config->basic_template.window_height, 0);
		init_assert(window, "创建游戏窗口失败！");

		SDL_PropertiesID props = SDL_CreateProperties();
		SDL_SetPointerProperty(props, SDL_PROP_RENDERER_CREATE_WINDOW_POINTER, window);//将窗口指针设置到属性中，供渲染器创建时使用
		SDL_SetNumberProperty(props, SDL_PROP_RENDERER_CREATE_PRESENT_VSYNC_NUMBER, 1);//启用垂直同步
		renderer = SDL_CreateRendererWithProperties(props);
		init_assert(renderer, "创建渲染器失败！");

		init_assert(ResourcesManager::instance()->load_from_file(renderer, mixer), "加载游戏资源失败！");

		init_assert(generate_tile_map_texture(), "生成瓦片地图失败");//初始化config_manager.h内置的SDL_FRect rect_tile_map
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
	MIX_Mixer* mixer = nullptr;

	SDL_Texture* tex_tile_map = nullptr;//永久保存整张地图纹理的核心纹理对象
private:
	void init_assert(bool flag, const char* err_msg)
	{
		if (flag) return;

		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "游戏启动失败", err_msg, window);
		exit(-1);
	}
	void on_input()
	{

	}
	void on_update(double delta)
	{

	}
	void on_render()
	{
		static ConfigManager* instance = ConfigManager::instance();
		static SDL_FRect& rect_dst = instance->rect_tile_map;

		SDL_RenderTexture(renderer, tex_tile_map, nullptr, &rect_dst);
	}
	
	bool generate_tile_map_texture()
	{
		const Map& map = ConfigManager::instance()->map;//这里的map理应是已经load("map.csv")的，不会在此处修改，所以是const类型的
		const TileMap& tile_map = map.get_tile_map();//const只获取map内置的tile_map，不修改它
		SDL_FRect& rect_tile_map = ConfigManager::instance()->rect_tile_map;//rect_tile_map是整张地图纹理在**窗口**上渲染的尺寸和位置，这里的引用&是为了修改
		SDL_Texture* tex_tile_set = ResourcesManager::instance()->get_texture_pool().find(ResID::Tex_Tileset)->second;

		float width_tex_tile_set, height_tex_tile_set;
		SDL_GetTextureSize(tex_tile_set, &width_tex_tile_set, &height_tex_tile_set);//获取resources文件里的tileset长宽
		int num_tile_single_line = std::ceil(width_tex_tile_set / SIZE_TILE);

		float width_tex_tile_map, height_tex_tile_map;
		width_tex_tile_map = (float)map.get_width() * SIZE_TILE;
		height_tex_tile_map = (float)map.get_height() * SIZE_TILE;
		tex_tile_map = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, 
			SDL_TEXTUREACCESS_TARGET, width_tex_tile_map, height_tex_tile_map);
		if (!tex_tile_map) return false;

		ConfigManager* config = ConfigManager::instance();
		rect_tile_map.x = (config->basic_template.window_width - width_tex_tile_map) / 2;
		rect_tile_map.y = (config->basic_template.window_height - height_tex_tile_map) / 2;
		rect_tile_map.w = width_tex_tile_map;
		rect_tile_map.h = height_tex_tile_map;

		SDL_SetTextureBlendMode(tex_tile_map, SDL_BLENDMODE_BLEND);//开启纹理的透明混合功能
		SDL_SetRenderTarget(renderer, tex_tile_map);//先把所有瓦片一次性画到一张大纹理上,之后每一帧只画这一张大纹理

		for (int y = 0; y < map.get_height(); y++)
		{
			for (int x = 0; x < map.get_width(); x++)
			{
				SDL_FRect rect_src;
				const Tile& tile = tile_map[y][x];

				const SDL_FRect rect_dst =
				{
					(float) x * SIZE_TILE,(float) y * SIZE_TILE,
					SIZE_TILE, SIZE_TILE
				};

				rect_src =
				{
					(float)(tile.terrain % num_tile_single_line) * SIZE_TILE,
					(float)(tile.terrain / num_tile_single_line) * SIZE_TILE,
					SIZE_TILE, SIZE_TILE
				};
				SDL_RenderTexture(renderer, tex_tile_set, &rect_src, &rect_dst);

				if (tile.decoration >= 0)
				{
					rect_src =
					{
						(float)(tile.decoration % num_tile_single_line) * SIZE_TILE,
						(float)(tile.decoration / num_tile_single_line) * SIZE_TILE,
						SIZE_TILE, SIZE_TILE
					};
					SDL_RenderTexture(renderer, tex_tile_set, &rect_src, &rect_dst);

				}
			}
		}

		const SDL_Point& idx_home = map.get_idx_home();
		const SDL_FRect rect_dst =
		{
			idx_home.x * SIZE_TILE, idx_home.y * SIZE_TILE,
			SIZE_TILE,SIZE_TILE
		};
		SDL_RenderTexture(renderer, ResourcesManager::instance()->get_texture_pool().find(ResID::Tex_Home)->second, nullptr, &rect_dst);

		SDL_SetRenderTarget(renderer, nullptr);

		return true;
	}
};
#endif // !_GAME_MANAGER_H_
