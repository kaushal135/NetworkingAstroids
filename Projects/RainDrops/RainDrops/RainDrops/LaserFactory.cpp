#include "GameCore.h"
#include "LaserFactory.h"
#include "PrefabAsset.h"
#include "AssetManager.h"
#include "InputManager.h"
#include "Transform.h"
#include "Laser.h";

IMPLEMENT_DYNAMIC_CLASS(LaserFactory);

using namespace std::placeholders;

void LaserFactory::initialize()
{
	Component::initialize();
	registerRPC(getHashCode("spawnLaser"), std::bind(&LaserFactory::spawnLaser, this, _1));
}

void LaserFactory::update(float deltaTime)
{
	Component::update(deltaTime);

	if (NetworkClient::Instance().isClient() == true)
	{
		return;
	}

}

void LaserFactory::load(XMLElement * element)
{
	Component::load(element);

	tinyxml2::XMLElement* prefabElement = element->FirstChildElement("PrefabAsset");
	THROW_RUNTIME_ERROR(prefabElement == nullptr, "No PrefabAsset element");
	const char* id = prefabElement->GetText();
	prefabID = getHashCode(id);
}

void LaserFactory::spawnLaser(RakNet::BitStream & bitStream)
{
	sf::Vector2f playerPos;
	bitStream.Read(playerPos.x);
	bitStream.Read(playerPos.y);

	if (NetworkServer::Instance().isServer()) {
		Asset* asset = AssetManager::Instance().getAsset(prefabID);
		if (asset != nullptr)
		{
			PrefabAsset* prefab = (PrefabAsset*)asset;
			GameObject* go = prefab->CreatePrefab();
			go->getTransform()->setPosition(playerPos);
		}
	}
}
