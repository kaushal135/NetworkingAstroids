#include "GameCore.h"
#include "PlayerFactory.h"
#include "InputManager.h"
#include "AssetManager.h"
#include "PrefabAsset.h"
#include "Transform.h"
#include "Player.h"
#include "InputController.h"

IMPLEMENT_DYNAMIC_CLASS(PlayerFactory)

using namespace std::placeholders;

void PlayerFactory::initialize()
{
	Component::initialize();
	registerRPC(getHashCode("spawnPlayer"), std::bind(&PlayerFactory::spawnPlayer, this, _1));
}

void PlayerFactory::load(XMLElement* element)
{
	Component::load(element);

	tinyxml2::XMLElement* prefabElement = element->FirstChildElement("PrefabAsset");
	THROW_RUNTIME_ERROR(prefabElement == nullptr, "No PrefabAsset element");
	const char* id = prefabElement->GetText();
	playerPrefabID[0] = getHashCode(id);

	prefabElement = prefabElement->NextSiblingElement("PrefabAsset");
	THROW_RUNTIME_ERROR(prefabElement == nullptr, "No PrefabAsset2 element");
	const char* id2 = prefabElement->GetText();
	playerPrefabID[1] = getHashCode(id2);

}

void PlayerFactory::spawnPlayer(RakNet::BitStream& bitStream)
{
	if (NetworkServer::Instance().isServer()) {
		if (numCurrentPlayer < 2) {
			Asset* asset = AssetManager::Instance().getAsset(playerPrefabID[numCurrentPlayer]);
			if (asset != nullptr)
			{
				PrefabAsset* prefab = (PrefabAsset*)asset;
				GameObject* go = prefab->CreatePrefab();

				playerPool[numCurrentPlayer] = go->getUID();

				auto gameObjects = GameObjectManager::Instance().GetAllRootGameObjects();
				for (auto gObject : gameObjects)
				{
					InputController* ctrl = dynamic_cast<InputController*>(
						gObject->GetComponentByUUID(InputController::getClassHashCode()));

					if (ctrl != nullptr)
					{

						RakNet::BitStream bitStream;
						bitStream.Write((unsigned char)ID_RPC_MESSAGE);
						bitStream.Write(gObject->getUID());
						bitStream.Write(InputController::getClassHashCode());
						bitStream.Write(getHashCode("setPlayerCallback"));
						bitStream.Write(go->getUID());
						NetworkServer::Instance().callRPC(bitStream, numCurrentPlayer);
					}
				}

				numCurrentPlayer++;
			}
		}
	}
}

void PlayerFactory::update(float deltaTime)
{
	Component::update(deltaTime);

	if (NetworkClient::Instance().isClient() && NetworkClient::Instance().getState() == NetworkClient::NetworkClientState::CONNECTED) {
		if (isSpawned[numCurrentPlayer] == false) {
			RakNet::BitStream bitStream;
			bitStream.Write((unsigned char)ID_RPC_MESSAGE);
			bitStream.Write(gameObject->getUID());
			bitStream.Write(PlayerFactory::getClassHashCode());
			bitStream.Write(getHashCode("spawnPlayer"));
			isSpawned[numCurrentPlayer] = true;
			NetworkClient::Instance().callRPC(bitStream);
		}

	}

}



