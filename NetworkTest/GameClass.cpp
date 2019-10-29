#include "GameClass.h"

GameClass::GameClass()
{
}

GameClass::~GameClass()
{
}

bool GameClass::Initialise()
{
	//Sets up the game stuff
	//Rect 1
	rect1.setSize(sf::Vector2f(100.0f, 100.0f));
	rect1.setFillColor(sf::Color::Red);
	rect1.setPosition(540.0f, 380.0f);

	//Rect 2
	rect2.setSize(sf::Vector2f(100.0f, 100.0f));
	rect2.setFillColor(sf::Color::Blue);
	rect2.setPosition(50.0f, 100.0f);

	//Font
	if (!font.loadFromFile("arial.ttf"))
	{
		throw std::runtime_error("Couldn't load from file");
	}

	banner.setFont(font);

	return true;
}

void GameClass::HandleInput()
{

}

void GameClass::Draw(sf::RenderWindow mWindow)
{
	//display text
	mWindow.draw(banner);

	mWindow.draw(rect1);
	mWindow.draw(rect2);

	mWindow.display();
	mWindow.clear();
}
