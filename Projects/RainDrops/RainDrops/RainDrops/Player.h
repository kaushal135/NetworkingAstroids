#pragma once

#include "Sprite.h"
#include <SFML\System.hpp>

class Player : public Sprite
{
	DECLARE_DYNAMIC_DERIVED_CLASS(Player, Sprite)

public:
	void initialize() override;
	virtual void update(float deltaTime) override;
	virtual void load(XMLElement* element) override;

	void writeCreate(RakNet::BitStream & bs) const override;
	void readCreate(RakNet::BitStream & bs) override;
};

