#include "GameClass.h"

GameClass::GameClass() : engine(seed_device()), distribution(-6, 6)
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
	idleRect = { 0, 0, 615, 564 }; //top row
	runRect = { 0, 564, 615, 564 }; //mid row
	attackRect = { 0, 1128, 615, 564 }; //bot row

	player.setTexture(playerTex);
	player.setTextureRect(idleRect);

	player.setOrigin(player.getTextureRect().width / 3, player.getTextureRect().height / 2);
	player.setScale(sf::Vector2f(0.5f, 0.5f));
	player.setPosition(300.0f, 250.0f);

	//Initialize ball
	InitBallMovement();

	//set up nd play music
	Sound();

	return true;
}

void GameClass::InitBallMovement()
{
	auto random = std::bind(distribution, std::ref(engine));

	direction = sf::Vector2f(random(), random());
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
	SetAnimState(0);

	//handle input of the player 1
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		SetAnimState(1);

		//move player 
		player.move(0.0f, -5.f); //-0.09f
	}

	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		//flipped running
		SetAnimState(3);

		//move player 
		player.move(-5.f, 0.0f);
	}

	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		SetAnimState(1);

		//move player 
		player.move(0.0f, 5.f);
	}

	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		SetAnimState(1);

		//move player 
		player.move(5.f, 0.0f);
	}

	//Hit the ball
	else if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		//run the attack animation
		hitFX.play();

		SetAnimState(2);
	}
}

void GameClass::BallMovement()
{
	//ball movement
	float force = 3.0f;
	bool hit = false;

	const auto pos = ball.getPosition();
	sf::Vector2f new_pos(pos.x + direction.x, pos.y + direction.y);

	//ball will move only if collision
	if (ball.getGlobalBounds().intersects(player.getGlobalBounds()))
	{
		//ball.move((force), 0.0f);

		hit = true;
	}

	if (new_pos.x + ball_radius >= (window_width - 50)) //blue rectangle
	{
		direction.x *= -1;
		new_pos.x = window_width - 50 - ball_radius;

		hit = false;
	}

	else if (new_pos.x - ball_radius < (0 + 20)) // red rectangle
	{
		direction.x *= -1;
		new_pos.x = 0 + 50 + ball_radius;

		hit = false;

	}

	else if (new_pos.y - ball_radius < 0) { // top of window
		direction.y *= -1;
		new_pos.y = 0 + ball_radius;

		hit = false;
	}

	else if (new_pos.y + ball_radius >= window_height) { // bottom of window
		direction.y *= -1;
		new_pos.y = window_height - ball_radius;

		hit = false;
	}

	ball.setPosition(new_pos);

	if (hit)
	{
		sNum += 1;
	}

	//update the score
	Score.setString(scoreNum + std::to_string(sNum));
}

void GameClass::AnimationHandler()
{
	static bool temp = false;

	if (timer.getElapsedTime().asSeconds() > 0.05f)
	{
		//Running
		if (GetAnimState() == 1)
		{
			if (runRect.left == 8610)
			{
				runRect.left = 0;
			}
			else
			{
				runRect.left += 615;
			}
			player.setTextureRect(runRect);
			
			player.setScale(sf::Vector2f(0.5f, 0.5f));
			temp = false;
		}

		//Flipped Running
		if (GetAnimState() == 3)
		{
			if (runRect.left == 8610)
			{
				runRect.left = 0;
			}
			else
			{
				runRect.left += 615;
			}
			player.setTextureRect(runRect);
			
			player.setScale(sf::Vector2f(-0.5f, 0.5f));
			temp = true;
		}

		//Attack
		if (GetAnimState() == 2)
		{
			if (attackRect.left == 8610)
			{
				attackRect.left = 0;
			}
			else
			{
				attackRect.left += 615;
			}

			player.setTextureRect(attackRect);
			player.setScale(sf::Vector2f(0.5f, 0.5f));
		}

		//IDLE
		if (GetAnimState() == 0)
		{
			if (idleRect.left == 8610)
			{
				idleRect.left = 0;
			}
			else
			{
				idleRect.left += 615;
			}

			player.setTextureRect(idleRect);
			if (temp)
				player.setScale(sf::Vector2f(-0.5f, 0.5f));
			else
				player.setScale(sf::Vector2f(0.5f, 0.5f));
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
