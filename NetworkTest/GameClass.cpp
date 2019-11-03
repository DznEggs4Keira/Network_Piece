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
	blueRect.setSize(sf::Vector2f(50.0f, 480.0f));
	blueRect.setFillColor(sf::Color::Blue);
	blueRect.setPosition(590.0f, 0.0f);

	//Rect 2
	redRect.setSize(sf::Vector2f(50.0f, 480.0f));
	redRect.setFillColor(sf::Color::Red);
	redRect.setPosition(0.0f, 0.0f);

	//Ball
	ball.setRadius(30.0f);
	ball.setFillColor(sf::Color::Magenta);
	ball.setPosition(320.0f, 240.0f);

	//Font
	if (!font.loadFromFile("data/arial.ttf"))
	{
		throw std::runtime_error("Couldn't load from file");
	}

	banner.setFont(font);

	//Sprite
	if (!playerTex.loadFromFile("data/Idle (1).png"))
	{
		throw std::runtime_error("Couldn't load from file");
	}

	player.setTexture(playerTex);
	player.setOrigin(sf::Vector2f(playerTex.getSize().x / 2, playerTex.getSize().y / 2));
	player.setScale(sf::Vector2f(0.5f, 0.5f));
	player.setPosition(320.0f, 280.0f);

	return true;
}

#pragma endregion

#pragma region Getter/Setter

sf::Vector2f GameClass::GetPlayerPos() { return player.getPosition(); }
void GameClass::SetPlayerPos(sf::Vector2f pr1) { player.setPosition(pr1); }

sf::Vector2f GameClass::GetBallPos() { return ball.getPosition(); }
void GameClass::SetBallPos(sf::Vector2f pr2) { ball.setPosition(pr2); }



#pragma endregion

#pragma region HandleInput

void GameClass::HandleInput()
{
	int state = 0;

	//handle input of the player 1
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		//move player 
		player.move(0.0f, -0.09f);

		state = 1;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		//move player 
		player.move(-0.09f, 0.0f);

		state = 2;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		//move player 
		player.move(0.0f, 0.09f);

		state = 3;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		//move player 
		player.move(0.09f, 0.0f);

		state = 4;
	}

	//Hit the ball
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		//run the attack animation
		BallMovement(state);
	}
}

void GameClass::BallMovement(int state)
{
	//ball movement
	float force = 0.05f;

	//ball will move only if collision
	if (ball.getGlobalBounds().intersects(player.getGlobalBounds()))
	{
		switch (state)
		{
			case 1:
			{
				ball.move(0.0f, -force);
				break;
			}
			case 2:
			{
				ball.move(-force, 0.0f);
				break;
			}
			case 3:
			{
				ball.move(0.0f, force);
				break;
			}
			case 4:
			{
				ball.move(force, 0.0f);
				break;
			}
		}
	}
}

#pragma endregion

#pragma region Render

void GameClass::Draw(sf::RenderWindow &mWindow)
{
	mWindow.draw(blueRect);
	mWindow.draw(ball);
	mWindow.draw(redRect);
	mWindow.draw(player);

	//display text
	mWindow.draw(banner);
}

#pragma endregion
