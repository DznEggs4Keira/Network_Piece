#pragma once

#include <random>
#include <functional>
#include <cstdlib>
#include <cmath>
#include <iostream>

//test game
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class GameClass
{
public:
	GameClass();
	~GameClass();

	bool Initialise();
	void HandleInput();
	void Draw(sf::RenderWindow &mWindow);

	//getter setters
	sf::Vector2f GetPlayerPos();
	void SetPlayerPos(sf::Vector2f);

	sf::Vector2f GetBallPos();
	void SetBallPos(sf::Vector2f);

	int GetScore() { return sNum; }
	void SetScore(int s) { sNum = s; SetScoreString(); }
	
	int GetAnimState() { std::cout << "pMove: " << pMove << std::endl; return pMove; }
	void SetAnimState(int pM) { pMove = pM; }

	void SetDelta(sf::Time tUpdate) { tDelta = tUpdate; }
	sf::Time GetDelta() { return tDelta; }

	void SetScoreString();

	void AnimationHandler();

	void BallMovement();

private:

	void InitBallMovement();

	void Sound();

private:
	//game objects
	sf::RectangleShape blueRect, redRect;

	//ball
	sf::CircleShape ball;
	//ball physics
	float window_width = 640.0f;
	float window_height = 480.0f;
	float ball_radius = 30.0f;

	std::random_device seed_device;
	std::default_random_engine engine;
	std::uniform_int_distribution<int> distribution;
	sf::Vector2f direction;
	const float velocity = std::sqrt(direction.x * direction.x + direction.y * direction.y);
	sf::Time tDelta;

	//player
	sf::Sprite player;
	sf::Texture playerTex;

	sf::Clock timer;
	sf::IntRect idleRect;
	sf::IntRect runRect;
	sf::IntRect attackRect;
	int pMove = 0;

	//Sound
	sf::Music BgMusic;
	sf::SoundBuffer buffer;
	sf::Sound hitFX;

public:
	//display text
	sf::Font font;
	sf::Text banner;

	sf::Text Score;
	std::string scoreNum = "Hit by Ball: ";
	int sNum = 0;
};

