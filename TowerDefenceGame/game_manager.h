#ifndef _GAME_MANAGER_H_
#define _GAME_MANAGER_H_

#include "manager.h"

class GameManager : public Manager<GameManager>
{
	friend class Manager<GameManager>;
public:
	
protected:
	GameManager() = default;
	~GameManager() = default;
	GameManager(const GameManager&) = delete;
	GameManager& operator=(const GameManager&) = delete;
};
#endif // !_GAME_MANAGER_H_
