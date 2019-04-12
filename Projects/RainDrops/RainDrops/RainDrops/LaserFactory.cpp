#include "GameCore.h"
#include "LaserFactory.h"
#include "PrefabAsset.h"
#include "Laser.h";

using namespace std::placeholders;
IMPLEMENT_DYNAMIC_CLASS(LaserFactory);

void LaserFactory::initialize()
{
	Component::initialize();
	registerRPC(getHashCode("spawnLaser"), std::bind(&LaserFactory::spawnLaser, this, _1));


}

void LaserFactory::update(float deltaTime)
{
	Component::update(deltaTime);

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
	if (NetworkServer::Instance().isServer()) {
		Asset* asset = AssetManager::Instance().getAsset(prefabID);
		if (asset != nullptr)
		{
			PrefabAsset* prefab = (PrefabAsset*)asset;
			GameObject* go = prefab->CreatePrefab();
			Laser* laser = (Laser*)go;
			laser->setSpeed(sf::Vector2f(0, 65));
		}
	}
}
