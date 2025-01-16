
#include <SFML/Graphics.hpp>
#include <iostream>

#include "GameManager.h"
#include "SampleScene.h"
#include "PVZScene.h"
#include "RugbyScene.h"

#include <cstdlib>
#include <crtdbg.h>

int main() 
{
    GameManager* pInstance = GameManager::Get();

	pInstance->CreateWindow(1280, 720, "Rugby", 60, sf::Color(13, 83, 1));
	
	pInstance->LaunchScene<RugbyScene>();

	return 0;
}