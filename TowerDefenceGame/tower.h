#ifndef  _TOWER_H_
#define _TOWER_H_

#include "vector2.h"
#include "animation.h"

class Tower
{
public:
	Tower(){}
	~Tower(){}

protected:
	Vector2 size;

	Animation anim_idle_up;
	Animation anim_idle_down;
	Animation anim_idle_left;
	Animation anim_idle_right;
	Animation anim_fire_up;
	Animation anim_fire_down;
	Animation anim_fire_left;
	Animation anim_fire_right;



private:

};

#endif // ! _TOWER_H_
