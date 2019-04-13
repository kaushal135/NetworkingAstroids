#pragma once

#include "Component.h"

class LaserFactory : public Component
{
	DECLARE_DYNAMIC_DERIVED_CLASS(LaserFactory, Component)

	enum NetworkPackets
	{
		SPAWN
	};

public:
	void initialize() override;
	virtual void update(float deltaTime);
	virtual void load(XMLElement* element);

	void spawnLaser(RakNet::BitStream& bitStream);

private:
	STRCODE prefabID;
};