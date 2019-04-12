#include "GameCore.h"
#include "Laser.h"
#include "PrefabAsset.h"
#include "Transform.h"

IMPLEMENT_DYNAMIC_CLASS(Laser)

void Laser::initialize()
{
	Sprite::initialize();

	
}

void Laser::update(float deltaTime)
{
	Sprite::update(deltaTime);
	if (gameObject->getTransform()->getPosition().y < -600)
	{
		gameObject->getTransform()->move(speed.x * deltaTime, speed.y * deltaTime);
	}
	else
	{
		GameObjectManager::Instance().DestroyGameObject(gameObject);
	}
}

void Laser::load(XMLElement * element)
{
	Sprite::load(element);

	XMLElement* maxElement = element->FirstChildElement("MaxSpeed");
	THROW_RUNTIME_ERROR(maxElement == nullptr, "No Max Element");
	speed = sf::Vector2f(maxElement->FloatAttribute("x"), maxElement->FloatAttribute("y"));
}

void Laser::writeCreate(RakNet::BitStream & bs) const
{
	Sprite::writeCreate(bs);
	bs.Write(speed.x);
	bs.Write(speed.y);
}

void Laser::readCreate(RakNet::BitStream & bs)
{
	Sprite::readCreate(bs);
	bs.Read(speed.x);
	bs.Read(speed.y);
}
