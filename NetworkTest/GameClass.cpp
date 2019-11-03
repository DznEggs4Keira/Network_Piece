#include "GameClass.h"

GameClass::GameClass()
{
}

GameClass::~GameClass()
{
}

#pragma region Initialise

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

#pragma endregion

#pragma region Getter/Setter

sf::Vector2f GameClass::GetR1() { return rect1.getPosition(); }
void GameClass::SetR1(sf::Vector2f pr1) { rect1.setPosition(pr1); }

sf::Vector2f GameClass::GetR2() { return rect2.getPosition(); }
void GameClass::SetR2(sf::Vector2f pr2) { rect2.setPosition(pr2); }

#pragma endregion

#pragma region HandleInput

void GameClass::HandleInput()
{
	//handle input of the player 1
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		//move player 1
		rect1.move(0.09f, 0.0f);

		//attempt mini collision detection
		//for player 2
		if (rect2.getGlobalBounds().intersects(rect1.getGlobalBounds()))
			rect2.move(0.05f, 0.0f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		//move player 1
		rect1.move(-0.09f, 0.0f);

		//attempt mini collision detection
		//for player 2
		if (rect2.getGlobalBounds().intersects(rect1.getGlobalBounds()))
			rect2.move(-0.05f, 0.0f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		//move player 1
		rect1.move(0.0f, -0.09f);

		//attempt mini collision detection
		//for player 2
		if (rect2.getGlobalBounds().intersects(rect1.getGlobalBounds()))
			rect2.move(0.0f, -0.05f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		//move player 1
		rect1.move(0.0f, 0.09f);

		//attempt mini collision detection
		//for player 2
		if (rect2.getGlobalBounds().intersects(rect1.getGlobalBounds()))
			rect2.move(0.0f, 0.05f);
	}
}

#pragma endregion

#pragma region Render

void GameClass::Draw(sf::RenderWindow &mWindow)
{
	//display text
	mWindow.draw(banner);

	mWindow.draw(rect1);
	mWindow.draw(rect2);
}

#pragma endregion
