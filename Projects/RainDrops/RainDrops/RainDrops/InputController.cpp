#include "GameCore.h"
#include "InputController.h"
#include "RainDrop.h"

IMPLEMENT_DYNAMIC_CLASS(InputController)

using namespace std::placeholders;

void InputController::initialize()
{
    Component::initialize();

    registerRPC(getHashCode("rpcCallback"), std::bind(&InputController::rpcCallback, this, _1));
}

void InputController::update(float deltaTime)
{
    Component::update(deltaTime);

    if (NetworkServer::Instance().isServer() == true || 
		NetworkClient::Instance().getState() != NetworkClient::NetworkClientState::CONNECTED)
    {
        return;
    }

    if (InputManager::Instance().mousePressed(sf::Mouse::Left))
    {
        RakNet::BitStream bitStream;
        bitStream.Write((unsigned char)ID_RPC_MESSAGE);
        bitStream.Write(gameObject->getUID());
        bitStream.Write(InputController::getClassHashCode());
        bitStream.Write(getHashCode("rpcCallback"));

        sf::Vector2f mousePosition = InputManager::Instance().getMousePosition();
        bitStream.Write(mousePosition.x);
        bitStream.Write(mousePosition.y);

        NetworkClient::Instance().callRPC(bitStream);
    }

	if (InputManager::Instance().keyPressed(sf::Keyboard::D)) {
		std::cout << "Hello world" << std::endl;
	}
}

void InputController::rpcCallback(RakNet::BitStream& bitStream)
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
