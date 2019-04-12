#include "GameCore.h"
#include "InputController.h"
#include "RainDrop.h"
#include "LaserFactory.h"

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

			RakNet::BitStream bitStream;
			bitStream.Write((unsigned char)ID_RPC_MESSAGE);
			bitStream.Write(gameObject->getUID());
			bitStream.Write(InputController::getClassHashCode());
			bitStream.Write(getHashCode("movePlayerCallback"));
			bitStream.Write(myShip->getShipUID());

			sf::Vector2f currentSpeed = myShip->getSpeed();
			if (currentSpeed.x <= myShip->getMaXSpeed().x) {
				currentSpeed.x += 10;
			}
			bitStream.Write(currentSpeed);
			NetworkClient::Instance().callRPC(bitStream);
		}

		if (InputManager::Instance().keyPressed(sf::Keyboard::A)) {
			std::cout << "Left" << std::endl;

			RakNet::BitStream bitStream;
			bitStream.Write((unsigned char)ID_RPC_MESSAGE);
			bitStream.Write(gameObject->getUID());
			bitStream.Write(InputController::getClassHashCode());
			bitStream.Write(getHashCode("movePlayerCallback"));
			bitStream.Write(myShip->getShipUID());

			sf::Vector2f currentSpeed = myShip->getSpeed();
			if (currentSpeed.x >= myShip->getMinSpeed().x) {
				currentSpeed.x -= 10;
			}
			bitStream.Write(currentSpeed);
			NetworkClient::Instance().callRPC(bitStream);
		}

		if (InputManager::Instance().keyPressed(sf::Keyboard::W)) {
			std::cout << "Up" << std::endl;
			RakNet::BitStream bitStream;
			bitStream.Write((unsigned char)ID_RPC_MESSAGE);
			bitStream.Write(gameObject->getUID());
			bitStream.Write(InputController::getClassHashCode());
			bitStream.Write(getHashCode("movePlayerCallback"));
			bitStream.Write(myShip->getShipUID());

			sf::Vector2f currentSpeed = myShip->getSpeed();
			if (currentSpeed.y >= myShip->getMinSpeed().y) {
				currentSpeed.y -= 10;
			}
			bitStream.Write(currentSpeed);
			NetworkClient::Instance().callRPC(bitStream);
		}

		if (InputManager::Instance().keyPressed(sf::Keyboard::S)) {
			std::cout << "Down" << std::endl;

			RakNet::BitStream bitStream;
			bitStream.Write((unsigned char)ID_RPC_MESSAGE);
			bitStream.Write(gameObject->getUID());
			bitStream.Write(InputController::getClassHashCode());
			bitStream.Write(getHashCode("movePlayerCallback"));
			bitStream.Write(myShip->getShipUID());

			sf::Vector2f currentSpeed = myShip->getSpeed();
			if (currentSpeed.y <= myShip->getMaXSpeed().y) {
				currentSpeed.y += 10;
			}
			bitStream.Write(currentSpeed);
			NetworkClient::Instance().callRPC(bitStream);
		}

		if (InputManager::Instance().keyPressed(sf::Keyboard::Space)) {
			std::cout << "Shoot" << std::endl;
			RakNet::BitStream bitStream;
			bitStream.Write((unsigned char)ID_RPC_MESSAGE);
			bitStream.Write(gameObject->getUID());
			bitStream.Write(LaserFactory::getClassHashCode());
			bitStream.Write(getHashCode("spawnLaser"));
			NetworkClient::Instance().callRPC(bitStream);


		}
	}

}

void InputController::movePlayerCallback(RakNet::BitStream& bitStream)
{
	STRCODE myShipID;
	bitStream.Read(myShipID);

	sf::Vector2f readSpeed;
	bitStream.Read(readSpeed);

	auto gameObjects = GameObjectManager::Instance().GetAllRootGameObjects();
	for (auto gameObject : gameObjects)
	{
		Player* plr = dynamic_cast<Player*>(
			gameObject->GetComponentByUUID(Player::getClassHashCode()));

		if (plr != nullptr)
		{
			if (gameObject->getUID() == myShipID)
			{
				plr->setSpeed(readSpeed);
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
			Player* plr = dynamic_cast<Player*>(
				go->GetComponentByUUID(Player::getClassHashCode()));

			if (plr != nullptr)
			{
				if (go->getUID() == myShipID)
				{
					myShip = plr;
				}
			}
		}
	}
}
