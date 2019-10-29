#pragma once

#include <iostream>

#include "GameClass.h"
#include "NetworkClass.h"

class SystemClass
{
public:
	SystemClass();
	~SystemClass();

	void Initialise();

	void Run();

private:
	GameClass* pGame;
	NetworkClass* pNetwork;
};

