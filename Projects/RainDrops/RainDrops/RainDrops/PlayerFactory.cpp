#include "GameCore.h"
#include "PlayerFactory.h"
#include "InputManager.h"
#include "AssetManager.h"
#include "PrefabAsset.h"
#include "Transform.h"
#include "Player.h"

IMPLEMENT_DYNAMIC_CLASS(PlayerFactory)

void PlayerFactory::initialize()
{
	Component::initialize();

	registerRPC(getHashCode("spawnPlayerRpcCallback"), std::bind(&PlayerFactory::spawnPlayerRpcCallback, this, _1));
}

void PlayerFactory::load(XMLElement* element)
{
	Component::load(element);

	tinyxml2::XMLElement* prefabElement = element->FirstChildElement("PrefabAsset");
	THROW_RUNTIME_ERROR(prefabElement == nullptr, "No PrefabAsset element");
	const char* id = prefabElement->GetText();
	playerPrefabID[0] = getHashCode(id);

	tinyxml2::XMLElement* prefab2Element = element->FirstChildElement("PrefabAsset2");
	THROW_RUNTIME_ERROR(prefab2Element == nullptr, "No PrefabAsset2 element");
	const char* id2 = prefab2Element->GetText();
	playerPrefabID[1] = getHashCode(id2);

}

void PlayerFactory::spawnPlayerRpcCallback(RakNet::BitStream& bitStream)
{
	if (numCurrentPlayer < 2) {
		Asset* asset = AssetManager::Instance().getAsset(playerPrefabID[numCurrentPlayer]);
		if (asset != nullptr)
		{
			PrefabAsset* prefab = (PrefabAsset*)asset;
			GameObject* go = prefab->CreatePrefab();
			numCurrentPlayer++;
		}
	}
}

void PlayerFactory::update(float deltaTime)
{
	Component::update(deltaTime);

	if (NetworkClient::Instance().isClient() == true)
	{	
		return;
	}



}



