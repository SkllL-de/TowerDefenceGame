#include <iostream>
#include "game_manager.h"
//Manager* Manager::manager = nullptr;

int main(int argc, char** argv)
{
	return GameManager::instance()->run(argc, argv);
}