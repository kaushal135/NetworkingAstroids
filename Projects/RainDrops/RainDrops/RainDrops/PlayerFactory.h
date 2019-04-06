#pragma once
#include "Component.h"

class PlayerFactory : public Component
{
	DECLARE_DYNAMIC_DERIVED_CLASS(PlayerFactory, Component)

	enum NetworkPackets
	{
		SPAWN
	};

public:
	virtual void update(float deltaTime);
	virtual void load(XMLElement* element);

private:
	int numCurrentPlayer = 0;
	STRCODE player1PrefabID;
	STRCODE player2PrefabID;
};

