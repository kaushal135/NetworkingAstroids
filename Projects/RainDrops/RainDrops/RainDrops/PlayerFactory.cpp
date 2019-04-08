#include "GameCore.h"
#include "PlayerFactory.h"
#include "InputManager.h"
#include "AssetManager.h"
#include "PrefabAsset.h"
#include "Transform.h"
#include "Player.h"

IMPLEMENT_DYNAMIC_CLASS(PlayerFactory)

using namespace std::placeholders;

void PlayerFactory::initialize()
{
	Component::initialize();
	numCurrentPlayer = 0;
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

void PlayerFactory::spawnPlayer()
{
	if (numCurrentPlayer < 2) {
		Asset* asset = AssetManager::Instance().getAsset(playerPrefabID[numCurrentPlayer]);
		if (asset != nullptr)
		{
			PrefabAsset* prefab = (PrefabAsset*)asset;
			GameObject* go = prefab->CreatePrefab();
			isSpawned[numCurrentPlayer] = true;
			numCurrentPlayer++;

		}
	}
}

void PlayerFactory::update(float deltaTime)
{
	Component::update(deltaTime);

	if (NetworkServer::Instance().isServer()) {
		if (isSpawned[numCurrentPlayer] == false && NetworkClient::Instance().getState() == NetworkClient::NetworkClientState::CONNECTED)
		{
			spawnPlayer();
		}
	}

}



