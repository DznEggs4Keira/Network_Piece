#include "NetworkClass.h"

#define PORT 2001

NetworkClass::NetworkClass()
{
}


NetworkClass::~NetworkClass()
{
}

#pragma region Initialise

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

#pragma endregion

#pragma region Calculations

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

#pragma endregion

#pragma region Getter/Setters

sf::Vector2f NetworkClass::GetP1() { return p1Position; }
void NetworkClass::SetP1(sf::Vector2f p1) {	p1Position = p1; }

sf::Vector2f NetworkClass::GetP2() { return p2Position; }
void NetworkClass::SetP2(sf::Vector2f p2) { p2Position = p2; }

sf::Vector2f NetworkClass::GetNP1() { return nP1Position; }
void NetworkClass::SetNP1(sf::Vector2f np1) { nP1Position = np1; }

sf::Vector2f NetworkClass::GetNP2() { return nP2Position; }
void NetworkClass::SetNP2(sf::Vector2f np2) { nP2Position = np2; }

sf::Vector2f NetworkClass::GetFP1() { return fP1Position; }
void NetworkClass::SetFP1(sf::Vector2f fp1) { fP1Position = fp1; }

sf::Vector2f NetworkClass::GetFP2() { return fP2Position; }
void NetworkClass::SetFP2(sf::Vector2f fp2) { fP2Position = fp2; }

int NetworkClass::GetSc1() { return sNum1; }
void NetworkClass::SetSc1(int Sc) { sNum1 = Sc; }

void NetworkClass::ConfirmConnect()
{
	//Sends Confirmation --- move to server and client
	testSocket.send(checkString.c_str(), checkString.length() + 1);

	testSocket.receive(buffer, sizeof(buffer), bufferSize);

	std::cout << buffer << std::endl;
}

void NetworkClass::SetString(GameClass* pGame)
{
	if(GetConnection() == 's')
	{
		//check for time at this instant
		serverTime = serverClock.getElapsedTime().asMilliseconds();

		//show time
		pGame->banner.setString("Server Time: " + std::to_string(serverTime));
	}

	if (GetConnection() == 'c')
	{
		//check clock for client
		clientTime = clientClock.getElapsedTime().asMilliseconds();

		//show time
		pGame->banner.setString("Client Time: " + std::to_string(clientTime + TimeLag + DeltaTime));
	}
}

#pragma endregion

#pragma region Server

void NetworkClass::ServerSide(GameClass* pGame, bool bUpdate)
{
	//Display Server Time
	SetString(pGame);

	//save the position vectors and Score into the vectors to send
	SetP1(pGame->GetPlayerPos());
	SetP2(pGame->GetBallPos());
	SetSc1(pGame->GetScore());

	//handle input and collision for player 1 and 2
	if (bUpdate)
	{
		pGame->HandleInput();
	}

	//first the server will always recieve time stamp from client
	//STEP 2 - Time Sync
	Step2TimeSync();

	//Clear packet
	testPacket.clear();

	//send position of player 1 and player 2 reaction......... every 2oms
	SendPosition(pGame);

	//can send many times but recieve only once so have to do this.
	testPacket.clear();
}

void NetworkClass::Step2TimeSync()
{
	// check the time stamp and then pack the position information and your own time and the time recieved from the client

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

					std::cout << "tempClient Time: " << tempClientTime << std::endl;
					//identifier << it's own time << the client time that came with it << position identifier << position information
					testPacket << timePack << serverTime << tempClientTime;
					//sending time
					testSocket.send(testPacket);

				}
			}
		}
	}
}

void NetworkClass::SendPosition(GameClass* pGame)
{
	//send position of player 1 and player 2 reaction......... every 2oms
	//this if means that if there is a change in position, send it as an update
	updatePosSent = posSentTime.getElapsedTime().asMilliseconds();
	if (updatePosSent >= 20)
	{
		if ((p1Position != pGame->GetPlayerPos()) || (p2Position != pGame->GetBallPos()) || sNum1 != pGame->GetScore())
		{
			//position identifier << latest serverTime << position information
			testPacket << PosPack << serverTime << pGame->GetPlayerPos().x << pGame->GetPlayerPos().y
				<< pGame->GetBallPos().x << pGame->GetBallPos().y << pGame->GetScore();

			sf::Socket::Status status = testSocket.send(testPacket);
			if (status == sf::Socket::Done)
			{
				std::cout << "Position Packet sent successfully from server" << std::endl;
			}
		}

		//reset the clock update posClock
		posSentTime.restart();
	}
}

#pragma endregion

#pragma region Client

void NetworkClass::ClientSide(GameClass* pGame)
{
	SetString(pGame);

	//this is where the time sync starts.
	//client will send a packet storing the identifier type and its client time
	//STEP 1 - time sync
	Step1TimeSync();

	//Clear Packet
	testPacket.clear();

	sf::Socket::Status status = testSocket.receive(testPacket);
	if (status == sf::Socket::Done)
	{
		//use the times to make a lag and calculate delta time
		if (testPacket >> check)
		{
			//first check is time
			if (check == 1)
			{
				//STEP 3 - time sync
				Step3TimeSync(pGame);
			}

			//second check is position
			if (check == 2)
			{
				RecievePosition(pGame);
			}
		}
		testPacket.clear();
	}

}

void NetworkClass::Step1TimeSync()
{
	//send current time of client
	//to loop it and check every 3seconds
	//send time packet every three seconds from client
	float threeSecond = timeToSync.getElapsedTime().asSeconds();
	if (threeSecond >= 3)
	{
		testPacket << timePack << clientTime;
		sf::Socket::Status status = testSocket.send(testPacket);
		if (status == sf::Socket::Done)
			std::cout << "Packet Sent" << std::endl;
			timeToSync.restart();
	}
}

void NetworkClass::Step3TimeSync(GameClass* pGame)
{
	//identifier is opened so first was server time and second was old client time
	if (testPacket >> tempServerTime >> oldClientTime)
	{
		//latest client time
		currentClientTime = clientClock.getElapsedTime().asMilliseconds();

		//calculate lag.... to do that we subtract the current client time from old client time recieved in the packet
		TimeLag = (currentClientTime - oldClientTime) / 2;

		//SumLag += TimeLag;

		//calculate delta time.... to do this we subtract the server time from the current client time
		DeltaTime = tempServerTime - currentClientTime;

		/*if (count == 3)
		//{
		//	//might need to change when prediction is happening
		//	AvgLag = SumLag / 3;
		//	std::cout << "Average Lag: " << AvgLag << std::endl;
		//	//clear sum lag to restart and find new average lag every 3 seconds
		//	SumLag = 0;
		//	count = 0;
		//}

		//else
		//	count++;*/
	}

}

void NetworkClass::RecievePosition(GameClass* pGame)
{
	if (testPacket >> tempServerTime >> nP1Position.x >> nP1Position.y >>
		nP2Position.x >> nP2Position.y >> sNum2)
	{
		//enter time into vector
		sTimeP1.push_back(tempServerTime);
		sTimeP2.push_back(tempServerTime);

		//enter position into vector, only handling nP1 for now
		positionP1.push_back(GetNP1());
		positionP2.push_back(GetNP2());

		if (((sTimeP1.size() >= 2) && (positionP1.size() >= 2)) && ((sTimeP2.size() >= 2) && (positionP2.size() >= 2)))
		{
			//get size of vectors first
			sizeT1 = sTimeP1.size();
			sizeD1 = positionP1.size();

			//get dDist for p1
			//p1 has its positions at 0 and 2 in vector
			dDistP1.x = positionP1.back().x - (positionP1.at(sizeD1 - 2).x);
			dDistP1.y = positionP1.back().y - (positionP1.at(sizeD1 - 2).y);

			dTimeP1 = sTimeP1.back() - (sTimeP1.at(sizeT1 - 2));

			//calculate velocity for p3
			velocityP1.x = dDistP1.x / dTimeP1;
			velocityP1.y = dDistP1.y / dTimeP1;

			//for p1
			SetFP1(Perdiction(velocityP1, sTimeP1.back(), positionP1.back()));
			//________________________________________________________________________________________________________________________

			sizeT2 = sTimeP2.size();
			sizeD2 = positionP2.size();

			//get dDist for p2
			//p2 has its positions at 1 and 3
			dDistP2.x = positionP2.back().x - (positionP2.at(sizeD2 - 2).x);
			dDistP2.y = positionP2.back().y - (positionP2.at(sizeD2 - 2).y);

			//just in case
			dTimeP2 = sTimeP2.back() - (sTimeP2.at(sizeT2 - 2));

			//calculate velocity for p2
			velocityP2.x = dDistP2.x / dTimeP2;
			velocityP2.y = dDistP2.y / dTimeP2;

			//for p2
			SetFP2(Perdiction(velocityP2, sTimeP2.back(), positionP2.back()));

			//place float value in here --- move makes it dissappear.
			pGame->SetPlayerPos(GetFP1());
			pGame->SetBallPos(GetFP2());
			pGame->SetScore(sNum2);
		}
	}
}

#pragma endregion
