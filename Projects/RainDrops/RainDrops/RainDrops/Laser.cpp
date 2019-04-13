#include "GameCore.h"
#include "Laser.h"
#include "PrefabAsset.h"
#include "Transform.h"
#include "RainDrop.h"

IMPLEMENT_DYNAMIC_CLASS(Laser)

void Laser::initialize()
{
	Sprite::initialize();
	topOfScreen = (RenderSystem::Instance().getView().getSize().y * -1.0f) - 64.0f;
}

void Laser::update(float deltaTime)
{
	Sprite::update(deltaTime);

	if (NetworkClient::Instance().isClient() == true)
	{
		return;
	}

	if (gameObject->getTransform()->getPosition().y > topOfScreen)
	{
		gameObject->getTransform()->move(speed.x * deltaTime, speed.y * deltaTime);
	}
	else
	{
		GameObjectManager::Instance().DestroyGameObject(gameObject);
	}

	if (NetworkServer::Instance().isServer())
	{
		checkCollisionWithAsteroids();
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

void Laser::checkCollisionWithAsteroids()
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
				rainDrop->DecreaseHealth();
				GameObjectManager::Instance().DestroyGameObject(gameObject);
				break;
			}
		}

	}
}