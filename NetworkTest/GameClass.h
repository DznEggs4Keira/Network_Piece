#pragma once
//test game
#include <SFML/Graphics.hpp>

class GameClass
{
public:
	GameClass();
	~GameClass();

	bool Initialise();
	void HandleInput();
	void Draw(sf::RenderWindow &mWindow);

	//getter setters
	sf::Vector2f GetR1();
	void SetR1(sf::Vector2f);

	sf::Vector2f GetR2();
	void SetR2(sf::Vector2f);

private:
	//game objects
	sf::RectangleShape rect1, rect2;

public:
	//display text
	sf::Font font;
	sf::Text banner;
};

