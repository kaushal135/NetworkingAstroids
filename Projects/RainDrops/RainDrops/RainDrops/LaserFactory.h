#pragma once
#include "Component.h"
class LaserFactory: public Component
{
	DECLARE_ABSTRACT_DERIVED_CLASS(LaserFactory, Component);

public:
	virtual void update(float deltaTime);
	virtual void load(XMLElement* element);
};

