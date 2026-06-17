#ifndef _COIN_MANAGER_H_
#define _COIN_MANAGER_H_

#include "coin_prop.h"
#include "manager.h"
#include "config_manager.h"

#include <vector>


class CoinManager : public Manager<CoinManager>
{
	friend class Manager<CoinManager>;

public:
	typedef std::vector<CoinProp*> CoinPropList;

public:
	void increase_coin(double val)
	{
		num_coin += val;
	}

	void decrease_coin(double val)
	{
		num_coin -= val;

		if (num_coin < 0)
			num_coin = 0;
	}

	void on_update(double delta)
	{
		for(CoinProp* coin_prop : coin_prop_list)
			coin_prop->on_update(delta);

		coin_prop_list.erase(std::remove_if(
			coin_prop_list.begin(), coin_prop_list.end(),
			[](CoinProp* coin_prop)
			{
				bool deleteable = coin_prop->can_remove();
				if (deleteable) delete coin_prop;
				return deleteable;
			}), coin_prop_list.end());


		static double passed_time = 0.0;
		passed_time += delta;

		if (passed_time >= 1.0)
		{
			increase_coin(1);
			passed_time -= 1.0;
		}
	}

	void on_render(SDL_Renderer* renderer)
	{
		for (CoinProp* coin_prop : coin_prop_list)
			coin_prop->on_render(renderer);
	}

	void on_reset()
	{
		num_coin = ConfigManager::instance()->num_initial_coin;
		for (CoinProp* coin_prop : coin_prop_list)
			coin_prop->make_invalid();
	}

	double get_current_coin_num()
	{
		return num_coin;
	}

	CoinPropList& get_coin_prop_list()
	{
		return coin_prop_list;
	}

	void spawn_coin_prop(const Vector2& position)
	{
		CoinProp* coin_prop = new CoinProp();
		coin_prop->set_position(position);

		coin_prop_list.push_back(coin_prop);
	}

protected:
	CoinManager()
	{
		num_coin = ConfigManager::instance()->num_initial_coin;
	}
	~CoinManager()
	{
		for (CoinProp* coin_prop : coin_prop_list)
			delete coin_prop;
		coin_prop_list.clear();
	}

private:
	double num_coin = 0;

	CoinPropList coin_prop_list;

};

#endif // !_COIN_MANAGER_H_
