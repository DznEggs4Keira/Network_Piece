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
	ball.setRadius(ball_radius);
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
	banner.setOutlineThickness(1.5f);
	banner.setOutlineColor(sf::Color::White);

	//Score board
	Score.setFont(font);
	Score.setPosition(sf::Vector2f(50.0f, 25.0f));
	Score.setFillColor(sf::Color::Black);
	Score.setStyle(sf::Text::Bold);
	Score.setOutlineThickness(1.5f);
	Score.setOutlineColor(sf::Color::White);

	Score.setString(scoreNum + std::to_string(sNum));

	//Sprite
	if (!playerTex.loadFromFile("data/FlatBoy.png"))
	{
		throw std::runtime_error("Couldn't load from file");
	}

	//set up the tex rect of the first frame of each type of animation
	idleRect = { 615, 0, 615, 564 }; //top row
	runRect = { 0, 564, 615, 564 }; //mid row
	attackRect = { 0, 1128, 615, 564 }; //bot row

	player.setTexture(playerTex);
	player.setTextureRect(idleRect);

	//player.setOrigin(sf::Vector2f(playerTex.getSize().x / 2, playerTex.getSize().y / 2)); --- Sprite dissapears
	player.setScale(sf::Vector2f(0.5f, 0.5f));
	player.setPosition(300.0f, 250.0f);

	//set up nd play music
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

void GameClass::SetScoreString()
{
	//update the score (For Client)
	Score.setString(scoreNum + std::to_string(GetScore()));
}

#pragma endregion

#pragma region HandleInput

void GameClass::HandleInput()
{
	//handle input of the player 1
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		//move player 
		player.move(0.0f, -0.09f);
	}

	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		//move player 
		player.move(-0.09f, 0.0f);
	}

	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		//move player 
		player.move(0.0f, 0.09f);
	}

	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		//move player 
		player.move(0.09f, 0.0f);
	}

	pMove = 1;

	//Hit the ball
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		//run the attack animation
		hitFX.play();

		pMove = 2;

		BallMovement(true);
	}
}

void GameClass::BallMovement(bool pressed)
{
	//ball movement
	float force = 0.5f;
	sf::Clock clock;
	float timer = clock.getElapsedTime().asSeconds();
	bool scored = false;

	/*
	std::random_device seed_device;
	std::default_random_engine engine(seed_device());
	std::uniform_int_distribution<int> distribution(-16, 16);
	auto random = std::bind(distribution, std::ref(engine));

	sf::Vector2f direction(random(), random());
	const float velocity = std::sqrt(direction.x * direction.x + direction.y * direction.y);

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

	elapsed += clock.restart();
	while (elapsed >= update_ms) {
		const auto pos = ball.getPosition();
		const auto delta = update_ms.asSeconds() * velocity;
		sf::Vector2f new_pos(pos.x + direction.x * delta, pos.y + direction.y * delta);

		// bounce back when hits the blue rect
		if (ball.getGlobalBounds().intersects(blueRect.getGlobalBounds()))
		{
			direction.x *= -1;
			new_pos.x = window_width - ball_radius;

			sNum += 1;
		}

		else if (new_pos.y - ball_radius < 0) { // top of window
			direction.y *= -1;
			new_pos.y = 0 + ball_radius;
		}
		else if (new_pos.y + ball_radius >= window_height) { // bottom of window
			direction.y *= -1;
			new_pos.y = window_height - ball_radius;
		}

		ball.setPosition(new_pos);

		elapsed -= update_ms;
	}
	*/

	//ball will move only if collision
	if (ball.getGlobalBounds().intersects(player.getGlobalBounds()))
	{
		ball.move((force), 0.0f);
	}

	if (ball.getGlobalBounds().intersects(blueRect.getGlobalBounds()))
	{
		ball.move(-2, 0.0f);

		scored = true;
	}

	if (scored)
	{
		sNum += 1;

		//reset scored
		scored = false;
	}

	//update the score
	Score.setString(scoreNum + std::to_string(sNum));
}

void GameClass::AnimationHandler()
{
	if (timer.getElapsedTime().asSeconds() > 0.05f)
	{
		switch (GetAnimState())
		{
			//Running
			case 1:
			{
				if (runRect.left == 8609)
				{
					runRect.left = 0;
				}
				else
				{
					runRect.left += 615;
				}

				player.setTextureRect(runRect);
				break;
			}

			//Attack
			case 2:
			{
				if (attackRect.left == 8609)
				{
					attackRect.left = 0;
				}
				else
				{
					attackRect.left += 615;
				}

				player.setTextureRect(attackRect);
				break;
			}

			//IDLE
			default:
			{
				if (idleRect.left == 8609)
				{
					idleRect.left = 0;
				}
				else
				{
					idleRect.left += 615;
				}

				player.setTextureRect(idleRect);
				break;
			}
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
