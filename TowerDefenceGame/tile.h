#ifndef _TILE_H_
#define _TILR_H_

#include <vector>

#define SIZE_TILE 48

struct Tile
{
	enum class Direction
	{
		None = 0,
		Up,
		Down,
		Left,
		Right
	};
	int terrain = 0;
	int decoration = -1;
	int special_flag = -1;
	//-1: 无特殊，0：防守目标点，1-10：敌人出生点
	bool has_tower = false;
	Direction direction = Direction::None;
};

typedef std::vector<std::vector<Tile>> TileMap;

#endif // !_TILE_H_

