#include "GameCore.h"
#include "GameEngine.h"

#include "RainDropFactory.h"
#include "RainDrop.h"
#include "InputController.h"
#include "PlayerFactory.h"
#include "Player.h"
#include "Laser.h"
#include "LaserFactory.h"

int main()
{
	// Register all custom components here
	REGISTER_DYNAMIC_CLASS(RainDropFactory);
    REGISTER_DYNAMIC_CLASS(RainDrop);
    REGISTER_DYNAMIC_CLASS(InputController);
	REGISTER_DYNAMIC_CLASS(PlayerFactory);
	REGISTER_DYNAMIC_CLASS(Player);
	REGISTER_DYNAMIC_CLASS(Laser);
	REGISTER_DYNAMIC_CLASS(LaserFactory);
	
	GameEngine::Instance().initialize();

	// initialize anything we need then call GameLoop

	GameEngine::Instance().GameLoop();
}

