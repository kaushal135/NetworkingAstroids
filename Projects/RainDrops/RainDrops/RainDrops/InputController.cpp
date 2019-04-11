#include "GameCore.h"
#include "InputController.h"
#include "RainDrop.h"

IMPLEMENT_DYNAMIC_CLASS(InputController)

using namespace std::placeholders;

void InputController::initialize()
{
    Component::initialize();

    registerRPC(getHashCode("movePlayerCallback"), std::bind(&InputController::movePlayerCallback, this, _1));
	registerRPC(getHashCode("setPlayerCallback"), std::bind(&InputController::setPlayerCallback, this, _1));
}

void InputController::update(float deltaTime)
{
    Component::update(deltaTime);

    if (NetworkServer::Instance().isServer() == true || 
		NetworkClient::Instance().getState() != NetworkClient::NetworkClientState::CONNECTED)
    {
        return;
    }

    /*if (InputManager::Instance().mousePressed(sf::Mouse::Left))
    {
        RakNet::BitStream bitStream;
        bitStream.Write((unsigned char)ID_RPC_MESSAGE);
        bitStream.Write(gameObject->getUID());
        bitStream.Write(InputController::getClassHashCode());
        bitStream.Write(getHashCode("movePlayerCallback"));

        sf::Vector2f mousePosition = InputManager::Instance().getMousePosition();
        bitStream.Write(mousePosition.x);
        bitStream.Write(mousePosition.y);

        NetworkClient::Instance().callRPC(bitStream);
    }*/

	if (myShip != nullptr) {
		if (InputManager::Instance().keyPressed(sf::Keyboard::D)) {
			std::cout << "Right" << std::endl;
		}
		if (InputManager::Instance().keyPressed(sf::Keyboard::A)) {
			std::cout << "Left" << std::endl;
		}
		if (InputManager::Instance().keyPressed(sf::Keyboard::W)) {
			std::cout << "Up" << std::endl;
		}
		if (InputManager::Instance().keyPressed(sf::Keyboard::S)) {
			std::cout << "Down" << std::endl;
		}
		if (InputManager::Instance().keyPressed(sf::Keyboard::Space)) {
			std::cout << "Shoot" << std::endl;
		}
	}
	
}

void InputController::movePlayerCallback(RakNet::BitStream& bitStream)
{
    sf::Vector2f mousePosition;
    bitStream.Read(mousePosition.x);
    bitStream.Read(mousePosition.y);

    auto gameObjects = GameObjectManager::Instance().GetAllRootGameObjects();
    for (auto gameObject : gameObjects)
    {
        RainDrop* rainDrop = dynamic_cast<RainDrop*>(
                             gameObject->GetComponentByUUID(RainDrop::getClassHashCode()));

        if (rainDrop != nullptr)
        {
            if (rainDrop->isWithinBounds(mousePosition.x, mousePosition.y))
            {
                GameObjectManager::Instance().DestroyGameObject(gameObject);
                break;
            }
        }
    }
}


void InputController::setPlayerCallback(RakNet::BitStream & bitStream)
{
	STRCODE myShipID;
	bitStream.Read(myShipID);

	if (myShipID != 0)
	{
		auto gameObjects = GameObjectManager::Instance().GetAllRootGameObjects();
		for (auto go : gameObjects)
		{
			Player* spaceship = dynamic_cast<Player*>(
				go->GetComponentByUUID(Player::getClassHashCode()));

			if (spaceship != nullptr)
			{
				if (go->getUID() == myShipID)
				{
					myShip = spaceship;
				}
			}
		}
	}
}
