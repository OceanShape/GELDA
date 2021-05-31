#include <iostream>

#include "Game.h"

int main() {

	Game gameExample("GELDA", 1024, 1024, "./data/resource.info", "./data/gameobject.info");

	gameExample.run();

	std::cout << "See U GELDA!" << std::endl;

	return 0;
}