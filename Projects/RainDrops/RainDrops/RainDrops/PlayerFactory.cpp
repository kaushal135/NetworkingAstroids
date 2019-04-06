#include "GameCore.h"
#include "PlayerFactory.h"
#include "InputManager.h"
#include "AssetManager.h"
#include "PrefabAsset.h"
#include "Transform.h"
#include "Player.h"

IMPLEMENT_DYNAMIC_CLASS(PlayerFactory)

void PlayerFactory::load(XMLElement* element)
{
	Component::load(element);

	tinyxml2::XMLElement* prefabElement = element->FirstChildElement("PrefabAsset");
	THROW_RUNTIME_ERROR(prefabElement == nullptr, "No PrefabAsset element");
	const char* id = prefabElement->GetText();
	player1PrefabID = getHashCode(id);

	tinyxml2::XMLElement* prefab2Element = element->FirstChildElement("PrefabAsset");
	THROW_RUNTIME_ERROR(prefab2Element == nullptr, "No PrefabAsset element");
	const char* id2 = prefab2Element->GetText();
	player2PrefabID = getHashCode(id2);

}

void PlayerFactory::update(float deltaTime)
{
	Component::update(deltaTime);



	if (NetworkClient::Instance().isClient() == true && 
		NetworkClient::Instance().getState() == NetworkClient::NetworkClientState::CONNECTED)
	{

		if (numCurrentPlayer == 0) {
			Asset* asset = AssetManager::Instance().getAsset(player1PrefabID);
			if (asset != nullptr)
			{
				PrefabAsset* prefab = (PrefabAsset*)asset;
				GameObject* go = prefab->CreatePrefab();
			}
		}
		else if (numCurrentPlayer == 1)
		{
			Asset* asset = AssetManager::Instance().getAsset(player2PrefabID);
			if (asset != nullptr)
			{
				PrefabAsset* prefab = (PrefabAsset*)asset;
				GameObject* go = prefab->CreatePrefab();
			}
		} 

		
	}

	

}



