#ifndef _TILE_H_
#define _TILE_H_

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
	int terrain = 0;//从0开始，地形像素图的索引
	int decoration = -1;//-1:无装饰，0及以后：装饰像素图的索引
	int special_flag = -1;//-1: 无特殊，0：防守目标点，1-10：敌人出生点
	bool has_tower = false;//是否有防御塔
	Direction direction = Direction::None;//到达这一个单元格后，下一步走向的方向
};

typedef std::vector<std::vector<Tile>> TileMap;

#endif // !_TILE_H_

