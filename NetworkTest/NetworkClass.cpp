#include "NetworkClass.h"

#define PORT 2001

NetworkClass::NetworkClass()
{
}


NetworkClass::~NetworkClass()
{
}

bool NetworkClass::Initialise()
{
	std::cout << "Enter <s> for server and <c> for client" << std::endl;
	std::cin >> connection;

	switch (connection)
	{
		case 's':
		{
			//server -- listen for a connection
			listener.listen(PORT);
			std::cout << "waiting for connection ..." << std::endl;

			listener.accept(testSocket);

			//should keep on saying waititng for connection until accepts socket
			checkString += "Server";
			mode = 's';
			break;
		}

		case 'c':
		{
			//mClient --- connects to a socket that the server will be listening to
			testSocket.connect(IP, PORT);
			checkString += "Client";
			mode = 's';
			break;
		}
	}

	return true;
}

sf::Vector2f NetworkClass::Perdiction(sf::Vector2f velocity, sf::Int32 serverTime, sf::Vector2f position)
{
	//T = serverTime - (clientTime + TimeLag + DeltaTime)
	//std::cout << "server Time: " << serverTime << std::endl;

	//std::cout << "Delta Time: " << DeltaTime << std::endl;
	//std::cout << "client Time: " << currentClientTime << std::endl;

	perdictionTime = serverTime - (currentClientTime + TimeLag + DeltaTime);
	std::cout << "perdiction Time: " << perdictionTime << std::endl;

	//a is velocity vector
	//b is position vector

	//equation is aT + b
	//for result make a temp vector
	sf::Vector2f tempVec;

	//acceleration should be divided by a value to make it smaller
	tempVec.x = ((velocity.x * perdictionTime) / 500.0f) + position.x;
	tempVec.y = ((velocity.y * perdictionTime) / 500.0f) + position.y;
	return tempVec;
}
