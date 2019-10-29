
#include "Network.h"
#include "SystemClass.h"

int main()
{
	/*
	std::cout << "welcome" << std::endl;

	Network test;
	test.run();
	*/

	//Init
	SystemClass* sys = new SystemClass;

	//Run
	sys->Run();

	//Shutdown
	delete sys;
	sys = nullptr;

	return 0;
}