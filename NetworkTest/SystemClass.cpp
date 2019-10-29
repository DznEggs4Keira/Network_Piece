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
	pGame = new GameClass;
	
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
	pNetwork->testSocket.send(pNetwork->checkString.c_str(), pNetwork->checkString.length() + 1);

	pNetwork->testSocket.receive(pNetwork->buffer, sizeof(pNetwork->buffer), pNetwork->bufferSize);

	std::cout << pNetwork->buffer << std::endl;

	//renders the window
	sf::RenderWindow mWindow(sf::VideoMode(640, 480, 32), "Network Test");

	//need this to make sure it recieves packets
	pNetwork->testSocket.setBlocking(false);

	//update movement of player
	bool update = false;

	/*this is the main game while loop.... here it will check based on connection
	and run the appropriate bit of the loop for for the server or client*/

	//handle game window
	while (mWindow.isOpen())
	{
		//check for server
		if (pNetwork->connection == 's')
		{
			//check for time at this instant
			pNetwork->serverTime = pNetwork->serverClock.getElapsedTime().asMilliseconds();

			//show time
			pGame->banner.setString("Server Time: " + std::to_string(pNetwork->serverTime));

			sf::Event Event;
			while (mWindow.pollEvent(Event))
			{
				if (Event.type == sf::Event::Closed /*|| Event.key.code == sf::Keyboard::Escape*/)
					mWindow.close();
				else if (Event.type == sf::Event::GainedFocus)
					update = true;
				else if (Event.type == sf::Event::LostFocus)
					update = false;
			}
			//save the position vectors into the vectors to send
			pNetwork->p1Position = pGame->rect1.getPosition();
			pNetwork->p2Position = pGame->rect2.getPosition();

			//handle input and collision for player 1 and 2
			if (update)
			{
				//handle input of the player 1
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
				{
					pGame->rect1.move(0.09f, 0.0f);
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
				{
					pGame->rect1.move(-0.09f, 0.0f);
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
				{
					pGame->rect1.move(0.0f, -0.09f);
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
				{
					pGame->rect1.move(0.0f, 0.09f);
				}

				//attempt mini collision detection
				//for player 2
				if (pGame->rect2.getGlobalBounds().intersects(pGame->rect1.getGlobalBounds()))
					pGame->rect2.move(0.05f, 0.0f); //right
			}

			//first the server will always recieve time stamp from client
			//STEP 2 - Time Sync

			// check the time stamp and the pack the position information and your own time and the time recieved from the client

			sf::Socket::Status status = pNetwork->testSocket.receive(pNetwork->testPacket);
			if (status == sf::Socket::Done)
			{

				if (pNetwork->testPacket >> pNetwork->check)
				{
					if (pNetwork->check == 1)
					{
						std::cout << "Packet Recieved" << std::endl;
						if (pNetwork->testPacket >> pNetwork->tempClientTime)
						{
							pNetwork->testPacket.clear();

							std::cout << "tempClient Time" << pNetwork->tempClientTime << std::endl;
							//identifier << it's own time << the client time that came with it << position identifier << position information
							pNetwork->testPacket << pNetwork->timePack << pNetwork->serverTime << pNetwork->tempClientTime;
							//sending time
							pNetwork->testSocket.send(pNetwork->testPacket);

						}
					}
				}
			}

			pNetwork->testPacket.clear();

			//send position of player 1 and player 2 reaction......... every 2oms
			//this if means that if there is a change in position, send it as an update
			float updatePosSent = pNetwork->posSentTime.getElapsedTime().asMilliseconds();
			if (updatePosSent >= 20)
			{
				if ((pNetwork->p1Position != pGame->rect1.getPosition()) || (pNetwork->p2Position != pGame->rect2.getPosition()))
				{
					//position identifier << latest pNetwork->serverTime << position information
					pNetwork->testPacket << pNetwork->PosPack << pNetwork->serverTime << pGame->rect1.getPosition().x << pGame->rect1.getPosition().y
						<< pGame->rect2.getPosition().x << pGame->rect2.getPosition().y;
					sf::Socket::Status status = pNetwork->testSocket.send(pNetwork->testPacket);

					if (status == sf::Socket::Done)
					{
						std::cout << "Position Packet sent successfully from server" << std::endl;
					}
				}

				//reset the clock update posClock
				pNetwork->posSentTime.restart();
			}

			//can send many times but recieve only once so have to do this.
			pNetwork->testPacket.clear();

		}

		//check for client
		if (pNetwork->connection == 'c')
		{
			//mClient.run();
			//check clock for client
			pNetwork->clientTime = pNetwork->clientClock.getElapsedTime().asMilliseconds();

			//show time
			pGame->banner.setString("Client Time: " + std::to_string(pNetwork->clientTime + pNetwork->TimeLag + pNetwork->DeltaTime));

			sf::Event Event;
			while (mWindow.pollEvent(Event))
			{
				if (Event.type == sf::Event::Closed /*|| Event.key.code == sf::Keyboard::Escape*/)
					mWindow.close();
			}

			//this is where the time sync starts.
			//client will send a packet storing the identifier type and its client time
			//STEP 1 - time sync
			//send current time of client
			//to loop it and check every 3seconds

			//pNetwork->testPacket.clear();

			//send time packet every three seconds from client
			float threeSecond = pNetwork->timeToSync.getElapsedTime().asSeconds();
			if (threeSecond >= 3)
			{
				pNetwork->testPacket << pNetwork->timePack << pNetwork->clientTime;
				sf::Socket::Status status = pNetwork->testSocket.send(pNetwork->testPacket);
				if (status == sf::Socket::Done)
					//std::cout << "Packet Sent" << std::endl;
					/*pNetwork->testPacket.clear();*/
					pNetwork->timeToSync.restart();
			}

			pNetwork->testPacket.clear();
			//___________________________________________________________________________

			//STEP 3 - time sync
			sf::Socket::Status status = pNetwork->testSocket.receive(pNetwork->testPacket);
			if (status == sf::Socket::Done)
			{
				//use the times to make a lag and calculate delta time
				if (pNetwork->testPacket >> pNetwork->check)
				{
					//first check is time
					if (pNetwork->check == 1)
					{
						//identifier is opened so first was server time and second was old client time
						if (pNetwork->testPacket >> pNetwork->tempServerTime >> pNetwork->oldClientTime)
						{
							//latest client time
							pNetwork->currentClientTime = pNetwork->clientClock.getElapsedTime().asMilliseconds();

							//calculate lag.... to do that we subtract the current client time from old client time recieved in the packet
							pNetwork->TimeLag = (pNetwork->currentClientTime - pNetwork->oldClientTime) / 2;

							//SumLag += TimeLag;

							//calculate delta time.... to do this we subtract the server time from the current client time
							pNetwork->DeltaTime = pNetwork->tempServerTime - pNetwork->currentClientTime;

							//if (count == 3)
							//{
							//	//might need to change when prediction is happening
							//	AvgLag = SumLag / 3;
							//	std::cout << "Average Lag: " << AvgLag << std::endl;
							//	//clear sum lag to restart and find new average lag every 3 seconds
							//	SumLag = 0;
							//	count = 0;
							//}

							//else
							//	count++;
						}
					}

					if (pNetwork->check == 2)
					{
						if (pNetwork->testPacket >> pNetwork->tempServerTime >> pNetwork->nP1Position.x >> pNetwork->nP1Position.y >> pNetwork->nP2Position.x >> pNetwork->nP2Position.y)
						{
							//enter time into vector
							pNetwork->sTimeP1.push_back(pNetwork->tempServerTime);
							pNetwork->sTimeP2.push_back(pNetwork->tempServerTime);

							//enter position into vector, only handling nP1 for now
							pNetwork->positionP1.push_back(pNetwork->nP1Position);
							pNetwork->positionP2.push_back(pNetwork->nP2Position);

							if (((pNetwork->sTimeP1.size() >= 2) && (pNetwork->positionP1.size() >= 2)) && ((pNetwork->sTimeP2.size() >= 2) && (pNetwork->positionP2.size() >= 2)))
							{
								//get size of vectors first
								pNetwork->sizeT1 = pNetwork->sTimeP1.size();
								pNetwork->sizeD1 = pNetwork->positionP1.size();

								//get dDist for p1
								//p1 has its positions at 0 and 2 in vector
								pNetwork->dDistP1.x = pNetwork->positionP1.back().x - (pNetwork->positionP1.at(pNetwork->sizeD1 - 2).x);
								pNetwork->dDistP1.y = pNetwork->positionP1.back().y - (pNetwork->positionP1.at(pNetwork->sizeD1 - 2).y);

								pNetwork->dTimeP1 = pNetwork->sTimeP1.back() - (pNetwork->sTimeP1.at(pNetwork->sizeT1 - 2));

								//calculate velocity for p3
								pNetwork->velocityP1.x = pNetwork->dDistP1.x / pNetwork->dTimeP1;
								pNetwork->velocityP1.y = pNetwork->dDistP1.y / pNetwork->dTimeP1;

								//for p1
								pNetwork->fP1Position = (pNetwork->Perdiction(pNetwork->velocityP1, pNetwork->sTimeP1.back(), pNetwork->positionP1.back()));
								//________________________________________________________________________________________________________________________

								pNetwork->sizeT2 = pNetwork->sTimeP2.size();
								pNetwork->sizeD2 = pNetwork->positionP2.size();

								//get dDist for p2
								//p2 has its positions at 1 and 3
								pNetwork->dDistP2.x = pNetwork->positionP2.back().x - (pNetwork->positionP2.at(pNetwork->sizeD2 - 2).x);
								pNetwork->dDistP2.y = pNetwork->positionP2.back().y - (pNetwork->positionP2.at(pNetwork->sizeD2 - 2).y);

								//just in case
								pNetwork->dTimeP2 = pNetwork->sTimeP2.back() - (pNetwork->sTimeP2.at(pNetwork->sizeT2 - 2));

								//calculate velocity for p2
								pNetwork->velocityP2.x = pNetwork->dDistP2.x / pNetwork->dTimeP2;
								pNetwork->velocityP2.y = pNetwork->dDistP2.y / pNetwork->dTimeP2;

								//for p2
								pNetwork->fP2Position = (pNetwork->Perdiction(pNetwork->velocityP2, pNetwork->sTimeP2.back(), pNetwork->positionP2.back()));

								//place float value in here --- move makes it dissappear.
								pGame->rect1.setPosition(pNetwork->fP1Position);
								pGame->rect2.setPosition(pNetwork->fP2Position);
							}
						}
					}
				}
				pNetwork->testPacket.clear();
			}

		}
	}
}