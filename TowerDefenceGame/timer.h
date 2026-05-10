#ifndef _TIMER_H_
#define _TIMER_H_

#include <functional>

class Timer
{
public:
	Timer() = default;
	~Timer() = default;

	void restart()//计时器状态重置
	{
		pass_time = 0;
		shotted = false;
	}

	void set_wait_time(double val)
	{
		wait_time = val;
	}

	void set_one_shot(bool flag)
	{
		one_shot = false;
	}

	void set_on_timeout(std::function<void()> on_timeout)
	{
		this->on_timeout = on_timeout;
	}

	void pause()
	{
		paused = true;
	}

	void resume()
	{
		paused = false;
	}

	void on_update(double delta)
	{
		if (paused) return;

		pass_time += delta;
		if (pass_time >= wait_time)
		{
			bool can_shot = (!one_shot) || (one_shot && !shotted);//当前可以再次触发
			shotted = true;
			if (can_shot && on_timeout)
				on_timeout();

			pass_time -= wait_time;
		}
	}

private:
	double pass_time = 0;//从计时器创建开始，经过的时间
	double wait_time = 0;//要等待的指定时间：经过多长时间，触发定时器的功能
	bool paused = false;//标注计时器是否暂停
	bool shotted = false;//定时器超时后（时间到达后），触发(shot)，bool型用于单次触发，表示是否触发过
	bool one_shot = false;//计时器是否单次触发
	std::function<void()> on_timeout;
};


#endif // !_TIMER_H_
