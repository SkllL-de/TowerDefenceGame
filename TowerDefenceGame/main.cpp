#include <iostream>
#include "manager.h"
#include "game_manager.h"
//Manager* Manager::manager = nullptr;

int main()
{
	GameManager* instance = GameManager::instance();

	std::cout << "Hello, World!" << std::endl;

	return 0;
}