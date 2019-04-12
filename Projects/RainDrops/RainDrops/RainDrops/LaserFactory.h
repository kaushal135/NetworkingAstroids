#pragma once

#include "Component.h"
#include <SFML/System.hpp>

class LaserFactory: public Component
{
	DECLARE_DYNAMIC_DERIVED_CLASS(LaserFactory, Component);

public:
	void initialize() override;
	virtual void update(float deltaTime);
	virtual void load(XMLElement* element);

	void spawnLaser(RakNet::BitStream& bitStream);
private:
	STRCODE prefabID;
};