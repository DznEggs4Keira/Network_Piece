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
	ball.setOutlineThickness(5.0f);
	ball.setOutlineColor(sf::Color::Color(245, 152, 66));
	ball.setPosition(320.0f, 240.0f);

	//Font
	if (!font.loadFromFile("data/arial.ttf"))
	{
		throw std::runtime_error("Couldn't load from file");
	}

	banner.setFont(font);
	banner.setPosition(sf::Vector2f(50.0f, 0.0f));
	banner.setFillColor(sf::Color::Black);
	banner.setStyle(sf::Text::Bold);
	banner.setOutlineThickness(2.0f);
	banner.setOutlineColor(sf::Color::White);

	//Score boards
	Score.setFont(font);
	Score.setPosition(sf::Vector2f(50.0f, 20.0f));
	Score.setFillColor(sf::Color::Black);
	Score.setStyle(sf::Text::Bold);
	Score.setOutlineThickness(2.0f);
	Score.setOutlineColor(sf::Color::White);

	Score.setString(scoreNum + std::to_string(sNum));

	//Sprite
	if (!playerTex.loadFromFile("data/FlatBoy.png"))
	{
		throw std::runtime_error("Couldn't load from file");
	}

	//set up the tex rect of the first frame of each type of animation
	idleRect = { 0, 0, 615, 564 };
	runRect = { 0, 564, 615, 564 };
	attackRect = { 0, 1128, 615, 564 };

	player.setTexture(playerTex);
	player.setTextureRect(idleRect);

	//player.setOrigin(sf::Vector2f(playerTex.getSize().x / 2, playerTex.getSize().y / 2)); --- Sprite dissapears
	player.setScale(sf::Vector2f(0.5f, 0.5f));
	player.setPosition(320.0f, 280.0f);

	Sound();

	return true;
}

void GameClass::Sound()
{
	if (!BgMusic.openFromFile("data/Background.wav"))
	{
		throw std::runtime_error("Couldn't open from file");
	}

	BgMusic.setLoop(true);
	BgMusic.setVolume(50);
	BgMusic.play();

	if (!buffer.loadFromFile("data/ballHit.wav"))
	{
		throw std::runtime_error("Couldn't load from file");
	}

	hitFX.setBuffer(buffer);
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
		state = 1;
		AnimationHandler(state);

		//move player 
		player.move(0.0f, -0.09f);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		state = 2;
		AnimationHandler(state);

		//move player 
		player.move(-0.09f, 0.0f);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		state = 3;
		AnimationHandler(state);

		//move player 
		player.move(0.0f, 0.09f);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		state = 4;
		AnimationHandler(state);

		//move player 
		player.move(0.09f, 0.0f);
	}

	//Hit the ball
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		//run the attack animation
		hitFX.play();
		AnimationHandler(5);

		BallMovement(state);

	}
}

void GameClass::BallMovement(int state)
{
	//ball movement
	float force = 0.5f;

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

	if (ball.getGlobalBounds().intersects(blueRect.getGlobalBounds()))
	{
		sNum += 1;
	}

	//update the score
	Score.setString(scoreNum + std::to_string(sNum));
}

void GameClass::AnimationHandler(int state)
{
	sf::Clock timer;

	if (timer.getElapsedTime().asSeconds() > 1.0f)
	{
		//IDLE
		if (state == 0)
		{
			if (idleRect.left == 9924)
			{
				idleRect.left = 0;
			}
			else
			{
				idleRect.left += 615;
			}

			player.setTextureRect(idleRect);
		}

		//Attack
		else if (state == 5)
		{
			if (attackRect.left == 9924)
			{
				attackRect.left = 0;
			}
			else
			{
				attackRect.left += 615;
			}

			player.setTextureRect(attackRect);
		}

		//Running
		else
		{
			if (runRect.left == 9924)
			{
				runRect.left = 0;
			}
			else
			{
				runRect.left += 615;
			}

			player.setTextureRect(runRect);
		}

		timer.restart();
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
	mWindow.draw(Score);
}

#pragma endregion
