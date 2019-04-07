#pragma once
#include "Component.h"

class PlayerFactory : public Component
{
	DECLARE_DYNAMIC_DERIVED_CLASS(PlayerFactory, Component)

	enum NetworkPackets
	{
		SPAWN
	};

public:

	inline static PlayerFactory& Instance()
	{
		static PlayerFactory instance;
		return instance;
	}

	void initialize() override;
	virtual void update(float deltaTime);
	virtual void load(XMLElement* element);

	void spawnPlayerRpcCallback(RakNet::BitStream& bitStream);



private:
	int numCurrentPlayer = 0;
	STRCODE playerPrefabID[2];

};

