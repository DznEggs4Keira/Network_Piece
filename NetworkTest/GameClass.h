#pragma once
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

private:
	void BallMovement(int state);
	void AnimationHandler(int state);
	void Sound();

private:
	//game objects
	sf::RectangleShape blueRect, redRect;

	//ball
	sf::CircleShape ball;

	//player
	sf::Sprite player;
	sf::Texture playerTex;

	sf::IntRect idleRect;
	sf::IntRect runRect;
	sf::IntRect attackRect;

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

