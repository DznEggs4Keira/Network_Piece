#include "Network.h"

#define PORT 2001

Network::Network()
{
}


Network::~Network()
{
}


void Network::run()
{
	//WELCOME SCREEN!!
	std::cout << "Enter <s> for server and <c> for client" << std::endl;
	std::cin >> connetion;

	if (connetion == 's')
	{
		//server -- listen for a connection
		listener.listen(PORT);
		std::cout << "waiting for connection ..." << std::endl;

		listener.accept(testSocket);
		
		//should keep on saying waititng for connection until accepts socket
		test += "Server";
		mode = 's';
	}
	else if (connetion == 'c')
	{
		//mClient --- connects to a socket that the server will be listening to
		testSocket.connect(IP, PORT);
		test += "Client";
		mode = 's';
	}

	//ANYTHING NOT BEING CHECKED BY AN IF SET IS BEING DONE BY BOTH CLIENT AND SERVER!

	//Sends Confirmation --- move to server and client
	testSocket.send(test.c_str(), test.length() + 1);

	testSocket.receive(buffer, sizeof(buffer), bufferSize);

	std::cout << buffer << std::endl;

	//text chat ---- can send basic messages to and from server and client
	//now this while is happening in both client and server....
	//and the mode switches the client and server to sending and recieving
	/*bool done = false;

	while (!done)
	{
		if (mode == 's')
		{
			std::getline(std::cin, test);
			testSocket.send(test.c_str(), test.length() + 1);
			if (test == "stop")
			{
				done = true;
			}

			else
			{
				mode = 'r';
			}
		}

		else if (mode == 'r')
		{
			testSocket.receive(buffer, sizeof(buffer), bufferSize);
			if(bufferSize > 0)
			{
				std::cout << buffer << std::endl;
				if (buffer == "stop")
				{
					done = true;
				}
				else
				{
					mode = 's';
				}
			}
		}
	}*/

	//if i send or recieve stop.. it should come out of the loop above

	//Sets up the game stuff --- game stuff will stay here.....or not?... hmmm
	sf::RectangleShape rect1, rect2;

	rect1.setSize(sf::Vector2f(100.0f, 100.0f));
	rect1.setFillColor(sf::Color::Red);
	rect1.setPosition(540.0f, 380.0f);

	rect2.setSize(sf::Vector2f(100.0f, 100.0f));
	rect2.setFillColor(sf::Color::Blue);
	rect2.setPosition(50.0f, 100.0f);

	if (!font.loadFromFile("arial.ttf"))
	{
		throw std::runtime_error("Couldn't load from file");
	}

	banner.setFont(font);


	//renders the window
	sf::RenderWindow mWindow(sf::VideoMode(640, 480, 32), "Network Test");

	//need this to make sure it recieves packets
	testSocket.setBlocking(false);

	//update movement of player
	bool update = false;

	/*this is the main game while loop.... here it will check based on connection
	and run the appropriate bit of the loop for for the server or client*/

	//handle game window
	while (mWindow.isOpen())
	{
		//check for server
		if (connetion == 's')
		{
			//check for time at this instant
			serverTime = serverClock.getElapsedTime().asMilliseconds();

			//show time
			banner.setString("Server Time: " + std::to_string(serverTime));

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
			p1Position = rect1.getPosition();
			p2Position = rect2.getPosition();

			//handle input and collision for player 1 and 2
			if (update)
			{
				//handle input of the player 1
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
				{
					rect1.move(0.09f, 0.0f);
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
				{
					rect1.move(-0.09f, 0.0f);
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
				{
					rect1.move(0.0f, -0.09f);
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
				{
					rect1.move(0.0f, 0.09f);
				}

				//attempt mini collision detection
				//for player 2
				if (rect2.getGlobalBounds().intersects(rect1.getGlobalBounds()))
					rect2.move(0.05f, 0.0f); //right
			}

			//first the server will always recieve time stamp from client
			//STEP 2 - Time Sync

			// check the time stamp and the pack the position information and your own time and the time recieved from the client

			sf::Socket::Status status = testSocket.receive(testPacket);
			if (status == sf::Socket::Done)
			{
				
				if (testPacket >> check)
				{
					if (check == 1)
					{
						std::cout << "Packet Recieved" << std::endl;
						if (testPacket >> tempClientTime)
						{
							testPacket.clear();

							std::cout << "tempClient Time" << tempClientTime << std::endl;
							//identifier << it's own time << the client time that came with it << position identifier << position information
							testPacket << timePack << serverTime << tempClientTime;
							//sending time
							testSocket.send(testPacket);

						}
					}
				}
			}

			testPacket.clear();

			//send position of player 1 and player 2 reaction......... every 2oms
			//this if means that if there is a change in position, send it as an update
			float updatePosSent = posSentTime.getElapsedTime().asMilliseconds();
			if (updatePosSent >= 20)
			{
				if ((p1Position != rect1.getPosition()) || (p2Position != rect2.getPosition()))
				{
					//position identifier << latest serverTime << position information
					testPacket << PosPack << serverTime << rect1.getPosition().x << rect1.getPosition().y
						<< rect2.getPosition().x << rect2.getPosition().y;
					sf::Socket::Status status = testSocket.send(testPacket);

					if (status == sf::Socket::Done)
					{
						std::cout << "Position Packet sent successfully from server" << std::endl;
					}
				}

				//reset the clock update posClock
				posSentTime.restart();
			}

			//can send many times but recieve only once so have to do this.
			testPacket.clear();

		}

		//check for client
		if (connetion == 'c')
		{
			//mClient.run();
			//check clock for client
			clientTime = clientClock.getElapsedTime().asMilliseconds();

			//show time
			banner.setString("Client Time: " + std::to_string(clientTime + TimeLag + DeltaTime));

			sf::Event Event;
			while (mWindow.pollEvent(Event))
			{
				if (Event.type == sf::Event::Closed /*|| Event.key.code == sf::Keyboard::Escape*/)
					mWindow.close();
			}

			//this is where the time sync starts.
			/*client will send a packet storing the identifier type and its client time*/
			//STEP 1 - time sync
			//send current time of client
			//to loop it and check every 3seconds

			//testPacket.clear();

			//send time packet every three seconds from client
			float threeSecond = timeToSync.getElapsedTime().asSeconds();
			if (threeSecond >= 3)
			{
				testPacket << timePack << clientTime;
				sf::Socket::Status status = testSocket.send(testPacket);
					if(status == sf::Socket::Done)
				//std::cout << "Packet Sent" << std::endl;
				/*testPacket.clear();*/
				timeToSync.restart();
			}

			testPacket.clear();
			//___________________________________________________________________________

			//STEP 3 - time sync
			sf::Socket::Status status = testSocket.receive(testPacket);
			if (status == sf::Socket::Done)
			{
			//use the times to make a lag and calculate delta time
				if (testPacket >> check)
				{
					//first check is time
					if (check == 1)
					{
						//std::cout << "Time Sync step 3: " << std::endl;
						//identifier is opened so first was server time and second was old client time
						if (testPacket >> tempServerTime >> oldClientTime)
						{
							//latest client time
							currentClientTime = clientClock.getElapsedTime().asMilliseconds();

							//std::cout << "old client time: " << oldClientTime << std::endl;
							//std::cout << "client time: " << currentClientTime << std::endl;

							//calculate lag.... to do that we subtract the current client time from old client time recieved in the packet
							TimeLag = (currentClientTime - oldClientTime) / 2;
							//std::cout << "Time Lag: " << TimeLag << std::endl;
							
							//SumLag += TimeLag;
							//std::cout << "Sum Lag: " << SumLag << std::endl;

							//calculate delta time.... to do this we subtract the server time from the current client time
							DeltaTime = tempServerTime - currentClientTime;
							//std::cout << "Delta Time: " << DeltaTime << std::endl;

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

					if (check == 2)
					{
						if (testPacket >> tempServerTime >> nP1Position.x >> nP1Position.y >> nP2Position.x >> nP2Position.y)
						{
							//enter time into vector
							sTimeP1.push_back(tempServerTime);
							sTimeP2.push_back(tempServerTime);

							//enter position into vector, only handling nP1 for now
							positionP1.push_back(nP1Position);
							positionP2.push_back(nP2Position);

							if (((sTimeP1.size() >= 2) && (positionP1.size() >= 2)) && ((sTimeP2.size() >= 2) && (positionP2.size() >= 2)))
							{
								//get size of vectors first
								sizeT1 = sTimeP1.size();
								sizeD1 = positionP1.size();

								//get dDist for p1
								//p1 has its positions at 0 and 2 in vector
								dDistP1.x = positionP1.back().x - (positionP1.at(sizeD1 - 2).x);
								//std::cout << "dDist.x" << dDist.x << std::endl;
								dDistP1.y = positionP1.back().y - (positionP1.at(sizeD1 - 2).y);
								//std::cout << "dDist.y" << dDist.y << std::endl;

								dTimeP1 = sTimeP1.back() - (sTimeP1.at(sizeT1 - 2));

								//calculate velocity for p3
								velocityP1.x = dDistP1.x / dTimeP1;
								velocityP1.y = dDistP1.y / dTimeP1;

								//std::cout << "velocity.x" << velocityP1.y << std::endl;
								//std::cout << "velocity.y" << velocityP1.y << std::endl;

								//for p1
								fP1Position = (perdiction(velocityP1, sTimeP1.back(), positionP1.back()));
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

								sizeT2 = sTimeP2.size();
								sizeD2 = positionP2.size();

								//get dDist for p2
								//p2 has its positions at 1 and 3
								dDistP2.x = positionP2.back().x - (positionP2.at(sizeD2 - 2).x);
								//std::cout << "dDist.x" << dDist.x << std::endl;
								dDistP2.y = positionP2.back().y - (positionP2.at(sizeD2 - 2).y);
								//std::cout << "dDist.y" << dDist.y << std::endl;

								//just in case
								dTimeP2 = sTimeP2.back() - (sTimeP2.at(sizeT2 - 2));

								//calculate velocity for p2
								velocityP2.x = dDistP2.x / dTimeP2;
								velocityP2.y = dDistP2.y / dTimeP2;

								//std::cout << "velocityP2.x" << velocityP2.y << std::endl;
								//std::cout << "velocityP2.y" << velocityP2.y << std::endl;

								//for p2
								fP2Position = (perdiction(velocityP2, sTimeP2.back(), positionP2.back()));

								std::cout << "Final applied position P1: " << fP1Position.x << ", " << fP1Position.y << std::endl;
								std::cout << "Final applied position P2: " << fP2Position.x << ", " << fP1Position.y << std::endl;
								
								//place float value in here --- move makes it dissappear.
								rect1.setPosition(fP1Position);
								rect2.setPosition(fP2Position);
							}
						}
					}
				}
				testPacket.clear();
			}

		}

		//display text
		mWindow.draw(banner);

		mWindow.draw(rect1);
		mWindow.draw(rect2);

		mWindow.display();
		mWindow.clear();
	}

	system("pause");
}

sf::Vector2f Network::perdiction(sf::Vector2f velocity, sf::Int32 serverTime, sf::Vector2f position)
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