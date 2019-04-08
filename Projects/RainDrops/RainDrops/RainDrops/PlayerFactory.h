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

	void initialize() override;
	virtual void update(float deltaTime);
	virtual void load(XMLElement* element);

	void spawnPlayer();



private:

	int numCurrentPlayer = 0;
	bool isSpawned[2] = { false, false };
	STRCODE playerPrefabID[2];

};

