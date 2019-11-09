#pragma once

#include <random>
#include <functional>
#include <cstdlib>
#include <cmath>

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

	void SetScoreString();

	void AnimationHandler();

private:
	int GetAnimState() { return pMove; }

	void BallMovement(bool);
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
	sf::Clock clock;
	sf::Time elapsed = clock.restart();
	const sf::Time update_ms = sf::seconds(1.f / 30.f);

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
	std::string scoreNum = "Score: ";
	int sNum = 0;
};

