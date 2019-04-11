#pragma once

#include "Component.h"
#include <SFML\System.hpp>
#include "Player.h"

class InputController : public Component
{
    DECLARE_DYNAMIC_DERIVED_CLASS(InputController, Component)

private:
	Player* myShip = nullptr;

public:
    void initialize() override;
    void update(float deltaTime) override;
	
    void rpcCallback(RakNet::BitStream& bitStream);
	void setPlayerCallback(RakNet::BitStream & bitStream);
};

