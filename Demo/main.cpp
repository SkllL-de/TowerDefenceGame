#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

#include <cJSON.h>
#include <SDL3/SDL.h>
#include <SDL3//SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_gfxPrimitives.h>

void test_json()
{
	std::ifstream file("test.json");
	if (!file.is_open())
	{
		std::cout << "无法打开文件" << std::endl;
		return;
	}
	std::stringstream str_stream;
	str_stream << file.rdbuf();
	file.close();

	cJSON* json_root = cJSON_Parse(str_stream.str().c_str());
	//std::cout << "JSON内容: " << cJSON_Print(json_root) << std::endl;
	cJSON* json_name = cJSON_GetObjectItem(json_root, "name");
	cJSON* json_age = cJSON_GetObjectItem(json_root, "age");
	cJSON* json_pets = cJSON_GetObjectItem(json_root, "pets");

	std::cout << json_name->string << ": " << json_name->valuestring << std::endl;
	std::cout << json_age->string << ": " << json_age->valueint << std::endl;

	std::cout << json_pets->string << ": " << std::endl;

	cJSON* json_item = nullptr;
	cJSON_ArrayForEach(json_item, json_pets)
	{
		std::cout << "\t" << json_item->valuestring << std::endl;
	}
}
void test_csv()
{
	std::ifstream file("test.csv");
	if(!file.is_open())
	{
		std::cout << "无法打开文件" << std::endl;
		return;
	}
	std::string str_line;
	while (std::getline(file, str_line))
	{
		std::string str_grid;
		std::stringstream str_stream(str_line);
		while (std::getline(str_stream, str_grid, ','))
		{
			std::cout << str_grid << " ";
		}
		std::cout << std::endl;
	}
	file.close();
}
int main(int argc, char* argv[])
{
	test_json();
	std::cout << "==============================" << std::endl;
	test_csv();
	//SDL_Init(SDL_INIT_EVERYTHING);
	//IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
	//Mix_Init(MIX_INIT_OGG | MIX_INIT_MP3);

	TTF_Init();
	MIX_Init();
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

	//创建窗口
	SDL_Window* window = SDL_CreateWindow("你好世界", 1280, 720, 0);
	//创建渲染器
	SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);

	//图片
	SDL_Surface* suf_img = IMG_Load("avatar.jpg");//加载图片到内存中，得到一个表面
	SDL_Texture* tex_img = SDL_CreateTextureFromSurface(renderer, suf_img);//将表面转换为纹理，得到一个纹理

	//字体
	TTF_Font* font = TTF_OpenFont("ipix.ttf", 32);//加载字体文件，得到一个字体
	SDL_Color color = { 255, 255, 255, 255 };//设置字体颜色为白色
	SDL_Surface* suf_text = TTF_RenderText_Blended(font, "你好世界", 0, color);//将文本渲染到表面上，得到一个表面
	SDL_Texture* tex_text = SDL_CreateTextureFromSurface(renderer, suf_text);//将表面转换为纹理，得到一个纹理

	//音频
	MIX_Mixer* mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);//创建一个混音器设备
	MIX_Audio* music = MIX_LoadAudio(mixer, "music.mp3", true);//加载音频文件到内存中，得到一个音频
	//音频的属性集options
	SDL_PropertiesID options = SDL_CreateProperties();//创建一个属性集
	SDL_SetNumberProperty(options, MIX_PROP_PLAY_LOOPS_NUMBER, -1);//无限循环
	SDL_SetNumberProperty(options, MIX_PROP_PLAY_FADE_IN_MILLISECONDS_NUMBER, 1500);//淡入时间1500毫秒
	MIX_Track* track = MIX_CreateTrack(mixer);//创建一个音频轨道
	MIX_SetTrackAudio(track, music);//将音频设置到轨道上
	MIX_PlayTrack(track, options);//播放轨道

	bool is_quit = false;

	SDL_Event event;
	SDL_Point pos_cursor = { 0, 0 };
	SDL_FRect rect_img = { 0, 0, suf_img->w, suf_img->h };
	SDL_FRect rect_text = { 0, 0, suf_text->w, suf_text->h };

	Uint64 last_counter = SDL_GetPerformanceCounter();
	Uint64 counter_frequence = SDL_GetPerformanceFrequency();

	while (!is_quit)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_EVENT_QUIT)
			{
				is_quit = true;
			}
			else if (event.type == SDL_EVENT_MOUSE_MOTION)
			{
				pos_cursor.x = event.motion.x;
				pos_cursor.y = event.motion.y;
			}
		}
		
		Uint64 current_counter = SDL_GetPerformanceCounter();
		double delta_time = static_cast<double>(current_counter - last_counter) / (counter_frequence);
		last_counter = current_counter;
		if (delta_time * 1000 < 1000.0 / 60)
		{
			//为了保持60帧每秒的帧率，如果当前帧的时间小于1/60秒(1000.0/60毫秒)，就让程序等待一段时间
			SDL_Delay(static_cast<Uint32>(1000.0 / 60 - delta_time * 1000));
		}
		//std::cout << "帧时间: " << delta_time * 1000 << " ms" << std::endl;
		//处理数据
		rect_img.x = pos_cursor.x - suf_img->w / 2;
		rect_img.y = pos_cursor.y - suf_img->h / 2;

		rect_text.x = pos_cursor.x - suf_text->w / 2;
		rect_text.y = pos_cursor.y - suf_text->h / 2 - suf_img->h / 2 - 10;

		 //渲染绘图
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);//设置渲染器的绘图颜色为黑色
		SDL_RenderClear(renderer);//清空渲染器

		SDL_RenderTexture(renderer, tex_img, nullptr, &rect_img);
		filledCircleRGBA(renderer, pos_cursor.x, pos_cursor.y, 50, 255, 0, 0, 155);//在鼠标位置绘制一个红色实心圆，半径为5
		SDL_RenderTexture(renderer, tex_text, nullptr, &rect_text);

		SDL_RenderPresent(renderer);
	}

	

	
	

	SDL_Quit();

	return 0;
}