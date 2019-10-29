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
	void Draw(sf::RenderWindow);

public: 
	//game objects
	sf::RectangleShape rect1, rect2;

	//display text
	sf::Font font;
	sf::Text banner;
};

