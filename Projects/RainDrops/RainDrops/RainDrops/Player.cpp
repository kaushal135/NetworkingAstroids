#include "GameCore.h"
#include "Player.h"
#include "PrefabAsset.h"
#include "Transform.h"
#include "RainDrop.h"

IMPLEMENT_DYNAMIC_CLASS(Player)
using namespace std::placeholders;

void Player::initialize()
{
	Sprite::initialize();

	if (NetworkServer::Instance().isServer())
	{
		speed.x = minSpeed.x;
		speed.y = minSpeed.y;
	}
	registerRPC(getHashCode("destroyPlayer"), std::bind(&Player::DestroyPlayer, this));
}

void Player::load(XMLElement* element)
{
	Sprite::load(element);

	XMLElement* minElement = element->FirstChildElement("MinSpeed");
	THROW_RUNTIME_ERROR(minElement == nullptr, "No Min Element");
	minSpeed = sf::Vector2f(minElement->FloatAttribute("x"), minElement->FloatAttribute("y"));

	XMLElement* maxElement = element->FirstChildElement("MaxSpeed");
	THROW_RUNTIME_ERROR(maxElement == nullptr, "No Max Element");
	maxSpeed = sf::Vector2f(maxElement->FloatAttribute("x"), maxElement->FloatAttribute("y"));
}

void Player::writeCreate(RakNet::BitStream & bs) const
{
	Sprite::writeCreate(bs);

	// Write the data needed
	bs.Write(maxSpeed.x);
	bs.Write(maxSpeed.y);
	bs.Write(minSpeed.x);
	bs.Write(minSpeed.y);

	bs.Write(speed.x);
	bs.Write(speed.y);
}

void Player::readCreate(RakNet::BitStream & bs)
{
	Sprite::readCreate(bs);

	// Read the data needed
	bs.Read(maxSpeed.x);
	bs.Read(maxSpeed.y);
	bs.Read(minSpeed.x);
	bs.Read(minSpeed.y);

	bs.Read(speed.x);
	bs.Read(speed.y);
}

const STRCODE Player::getShipUID() const
{
	return gameObject->getUID();
}

const void Player::DestroyPlayer()
{
	GameObjectManager::Instance().DestroyGameObject(gameObject);
}

void Player::checkCollisionWithAsteroids()
{
	auto gameObjects = GameObjectManager::Instance().GetAllRootGameObjects();
	for (auto go : gameObjects)
	{
		RainDrop* rainDrop = dynamic_cast<RainDrop*>(
			go->GetComponentByUUID(RainDrop::getClassHashCode()));
		
		if (rainDrop != nullptr)
		{
			if (rainDrop->isWithinBounds(gameObject->getTransform()->getPosition().x, gameObject->getTransform()->getPosition().y))
			{
				RakNet::BitStream bitStream;
				bitStream.Write((unsigned char)ID_RPC_MESSAGE);
				bitStream.Write(gameObject->getUID());
				bitStream.Write(Player::getClassHashCode());
				bitStream.Write(getHashCode("destroyPlayer"));
				NetworkClient::Instance().callRPC(bitStream);
			}
		}
		
	}
}



void Player::update(float deltaTime)
{
	Sprite::update(deltaTime);

	if (gameObject->getTransform()->getPosition().y < 50)
	{
		gameObject->getTransform()->move(speed.x * deltaTime, speed.y * deltaTime);
	}
	else
	{
		GameObjectManager::Instance().DestroyGameObject(gameObject);
	}
	checkCollisionWithAsteroids();

}

