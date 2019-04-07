#pragma once

#include "Sprite.h"
#include <SFML\System.hpp>

class Player : public Sprite
{
	DECLARE_DYNAMIC_DERIVED_CLASS(Player, Sprite)

	enum moveDirections
	{
		Up,
		Down,
		Left,
		Right
	};

public:
	void initialize() override;
	virtual void update(float deltaTime) override;
	virtual void load(XMLElement* element) override;

	void writeCreate(RakNet::BitStream & bs) const override;
	void readCreate(RakNet::BitStream & bs) override;

	void moveRpcCallback(moveDirections dir);

private:
	sf::Vector2f speed;
	sf::Vector2f minSpeed;
	sf::Vector2f maxSpeed;
};

