#ifndef _AXE_BULLET_H_
#define _AXE_BULLET_H_

#include "bullet.h"
#include "resources_manager.h"
#include "audio_manager.h"

class AxeBullet : public Bullet
{
public:
	AxeBullet()
	{
		static SDL_Texture* tex_axe = ResourcesManager::instance()
			->get_texture_pool().find(ResID::Tex_BulletAxe)->second;

		static const std::vector<int> idx_list = { 0, 1, 2, 3, 4, 5, 6, 7 };

		animation.set_loop(true);
		animation.set_interval(0.1);
		animation.set_frame_data(tex_axe, 4, 2, idx_list);

		size.x = 48, size.y = 48;
	}

	~AxeBullet() = default;

	void on_collide(Enemy* enemy) override
	{
		static const ResourcesManager::AudioPool& audio_pool
			= ResourcesManager::instance()->get_audio_pool();

		AudioManager* audio_manager = AudioManager::instance();

		switch (rand() % 3)
		{
		case 0:
			audio_manager->PlayMusic(audio_pool.find(ResID::Sound_AxeHit_1)->second);
			break;
		case 2:
			audio_manager->PlayMusic(audio_pool.find(ResID::Sound_AxeHit_2)->second);
			break;
		case 3:
			audio_manager->PlayMusic(audio_pool.find(ResID::Sound_AxeHit_3)->second);
			break;
		}

		enemy->slow_down();

		Bullet::on_collide(enemy);
	}

};

#endif // !_AXE_BULLET_H_
