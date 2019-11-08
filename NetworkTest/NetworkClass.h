#pragma once

#include <SFML/Network.hpp>

#include <string>
#include <iostream>
#include <vector>

#include "GameClass.h"

class NetworkClass
{
public:
	NetworkClass();
	~NetworkClass();

	//Init
	bool Initialise();

	//getter setters
	sf::Vector2f GetP1(); void SetP1(sf::Vector2f);

	sf::Vector2f GetP2(); void SetP2(sf::Vector2f);

	sf::Vector2f GetNP1(); void SetNP1(sf::Vector2f);

	sf::Vector2f GetNP2(); void SetNP2(sf::Vector2f);

	sf::Vector2f GetFP1(); void SetFP1(sf::Vector2f);

	sf::Vector2f GetFP2(); void SetFP2(sf::Vector2f);

	int GetSc1(); void SetSc1(int);

	//check what connection, server or client
	char GetConnection() { return connection; }
	//Confirmation on connection String
	void ConfirmConnect();

	//Server Side
	void ServerSide(GameClass* pGame, bool bUpdate);

	//Client Side
	void ClientSide(GameClass* pGame);

private:
	//output the string to show Client/Server Time
	void SetString(GameClass* pGame);

	//Server Side
		//Time Sync
	//Recieve Time Pack
	void Step2TimeSync();
	void SendPosition(GameClass* pGame);

	//Client Side
		//Time Sync
	//Send Time Pack
	void Step1TimeSync();
	//Recieve Time Pack
	void Step3TimeSync(GameClass* pGame);
	void RecievePosition(GameClass* pGame);

	//Calc
	sf::Vector2f Perdiction(sf::Vector2f velocity, sf::Int32 serverTime, sf::Vector2f position);

private:
	//positions of the game players --- needed for sending positions
	//server
	sf::Vector2f p1Position, p2Position;
	//client
	sf::Vector2f nP1Position, nP2Position;

	//final positions actually applied
	sf::Vector2f fP1Position, fP2Position;

	//score recieved from server
	int sNum1, sNum2;

	char connection, mode;
	//recieve buffer
	char buffer[200];
	size_t bufferSize;

	//for server --- listener is the socket for the server and test socket is the socket the client will connect to
	//for a successful connection, listener needs to accept the test socket.
	sf::TcpListener listener;
	sf::IpAddress IP = sf::IpAddress::getLocalAddress();

	//packet will store time and position
	sf::Packet testPacket;

	//to let the server know that this is a time packet
	sf::Int16 check;
	//values sent with the packet
	sf::Int16 timePack = 1;
	sf::Int16 PosPack = 2;


	//clocks for server
	sf::Clock serverClock;
	sf::Clock posSentTime;
	//used to store clock because using clock itself will cause problems
	sf::Int32 serverTime;
	sf::Int32 tempClientTime;

	//client clock
	sf::Clock clientClock;
	sf::Clock timeToSync;
	//used to store clock because using clock itself will cause problems
	sf::Int32 clientTime;
	sf::Int32 currentClientTime;
	sf::Int32 tempServerTime;
	sf::Int32 oldClientTime;
	sf::Int32 newClientTime;

	//to get the avg lag
	sf::Int32 SumLag;

	//handling time sync
	sf::Int32 TimeLag;
	sf::Int32 AvgLag;
	sf::Int32 DeltaTime;

	int count = 0;

	// vector for time and position
	std::vector <sf::Vector2f> positionP1;
	std::vector <sf::Int32> sTimeP1;

	std::vector <sf::Vector2f> positionP2;
	std::vector <sf::Int32> sTimeP2;

	//sizes to get second last values
	int sizeD1;
	int sizeT1;

	int sizeD2;
	int sizeT2;

	//deltas to calculate velocity
	//try with sf vector... if not then do seperately with float
	sf::Vector2f dDistP1;
	sf::Int32 dTimeP1;
	sf::Vector2f dDistP2;
	sf::Int32 dTimeP2;
	sf::Vector2f velocityP1;
	sf::Vector2f velocityP2;

	//T from aT + b of prediction
	sf::Int32 perdictionTime;

	std::string checkString = "Connected to ";

	//new variables
	float updatePosSent;

public:
	//socket
	sf::TcpSocket testSocket;
};

