#include "SystemClass.h"


SystemClass::SystemClass()
{
	pGame = nullptr;
	pNetwork = nullptr;

	Initialise();
}

SystemClass::~SystemClass()
{
	if (pGame)
	{
		delete pGame;
		pGame = nullptr;
	}

	if (pNetwork)
	{
		delete pNetwork;
		pNetwork = nullptr;
	}
}

void SystemClass::Initialise()
{
	bool result;

	//Init Network
	pNetwork = new NetworkClass;

	result = pNetwork->Initialise();
	if (!result)
	{
		std::cout << "Could not initialise Network" << std::endl;
	}

	//Init Game
	pGame = new GameClass();
	
	result = pGame->Initialise();
	if (!result)
	{
		std::cout << "Could not initialise Game Window" << std::endl;
	}

}

void SystemClass::Run()
{
	std::cout << "Hello World!" << std::endl;

	//Sends Confirmation --- move to server and client
	pNetwork->ConfirmConnect();

	sf::RenderWindow mWindow(sf::VideoMode(640, 480, 32), "Network Piece");

	//Prepare the socket to recieve the packets
	pNetwork->testSocket.setBlocking(false);

	//update movement of player
	bool update = false;

	/*this is the main game while loop.... here it will check based on connection
	and run the appropriate bit of the loop for for the server or client*/
	sf::Clock clock;
	sf::Time elapsed = clock.restart();
	const sf::Time update_ms = sf::seconds(1.f / 120.f);

	//handle game window
	while (mWindow.isOpen())
	{
			//if server
		if(pNetwork->GetConnection() == 's')
		{
			//Poll events
			sf::Event Event;
			while (mWindow.pollEvent(Event))
			{
				if (Event.type == sf::Event::Closed /*|| Event.key.code == sf::Keyboard::Escape*/)
					mWindow.close();
				//makes sure that the user input is only working when in focus
				else if (Event.type == sf::Event::GainedFocus)
					update = true;
				else if (Event.type == sf::Event::LostFocus)
					update = false;
			}

			elapsed += clock.restart();
			while (elapsed >= update_ms)
			{
				pNetwork->ServerSide(pGame, update, update_ms);

				elapsed -= update_ms;
			}
		}

			//if client
		if (pNetwork->GetConnection() == 'c')
		{
			//Poll events
			sf::Event Event;
			while (mWindow.pollEvent(Event))
			{
				if (Event.type == sf::Event::Closed /*|| Event.key.code == sf::Keyboard::Escape*/)
					mWindow.close();
			}

			elapsed += clock.restart();
			while (elapsed >= update_ms)
			{
				pNetwork->ClientSide(pGame);

				elapsed -= update_ms;
			}
		}

		//Finally draw elements of window and then display
		pGame->Draw(mWindow);
		mWindow.display();

		//Clear the window for the next frame
		mWindow.clear(sf::Color::Green);
	}

	//pause the application on exit from window
	//system("pause");
}