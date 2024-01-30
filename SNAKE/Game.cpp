#include "Game.h"

void Game::initMap()
{
	this->map.push_back(std::vector<Tile*>());
	for (int x = 0; x < this->sizeX; x++)
	{
		this->map.push_back(std::vector<Tile*>());
		for (int y = 0; y < this->sizeY; y++)
		{	
			this->map[x].push_back(nullptr);
			this->map[x][y] = new Tile(x * this->size, y * this->size, this->size);
		}
	}
}

void Game::initSnake()
{
	this->snake = new Snake(this->size);
}

void Game::deleteMap()
{
	//delete map
	for (int x = 0; x < this->sizeX; x++)
	{
		for (int y = 0; y < this->sizeY; y++)
		{
			delete this->map[x][y];
		}
	}
}

Game::Game()
{
	this->size = 50.f;
	this->sizeX = 15;
	this->sizeY = 15;

	this->window = new sf::RenderWindow(sf::VideoMode(this->sizeX * this->size, this->sizeY * this->size), "My window");
	this->maxDt = 120.f;
	this->dt = this->maxDt;
	
	this->initMap();
	this->initSnake();
	this->apple = new sf::RectangleShape(sf::Vector2f(this->size, this->size));
	this->apple->setPosition(5.f * this->size, 5.f * this->size);
	this->apple->setFillColor(sf::Color(139, 0, 0));
}

Game::~Game()
{
	delete this->apple;
	delete this->snake;
	this->deleteMap();
	delete this->window;
}

void Game::SpawnApple()
{	
	int x = rand() % this->sizeX;
	int y = rand() % this->sizeY;

	this->apple->setPosition(static_cast<float>(x) * this->size, static_cast<float>(y) * this->size);
}

void Game::updateCollsion()
{
	if (this->snake->body[0].getPosition() == this->apple->getPosition())
	{
		sf::RectangleShape node;
		node.setPosition(
			this->snake->body[this->snake->size_itself - 1].getPosition().x,
			this->snake->body[this->snake->size_itself - 1].getPosition().y
		);
		node.setFillColor(sf::Color(139, 0, 0));
		node.setSize(sf::Vector2f(size, size));
		this->snake->body.push_back(node);
		this->snake->size_itself++;
		this->maxDt -= 5;
		this->SpawnApple();
	}

	for (int i = 1; i < this->snake->size_itself; i++)
	{
		if (this->snake->body[0].getPosition() == this->snake->body[i].getPosition())
		{	
			for (int i = this->snake->size_itself - 1; i > 0; i--)
			{
				if (this->snake->body[i].getPosition() != this->snake->body[0].getPosition())
				{
					this->snake->body.pop_back();
					this->snake->size_itself--;
					this->maxDt += 5.f;
				}
			}
			/*std::cout << "col" << '\n';*/
		}
	}

	for (auto& node : this->snake->body)
	{
		if (node.getPosition().x == this->size * this->sizeX)
		{
			node.setPosition(0.f, node.getPosition().y);
		}

		if (node.getPosition().x == -this->size)
		{
			node.setPosition(this->size * (this->sizeX - 1), node.getPosition().y);
		}

		if (node.getPosition().y == this->size * this->sizeY)
		{
			node.setPosition(node.getPosition().x, 0.f);
		}

		if (node.getPosition().y == -this->size)
		{
			node.setPosition(node.getPosition().x, this->size * (this->sizeY - 1));
		}
	}

}

void Game::renderApple()
{
	this->window->draw(*this->apple);
}

void Game::updateDt()
{
	this->dt += 1.f;
	if(this->dt >= this->maxDt)
	{
		this->dt = 0.f;
	}
	
}

void Game::updatePollEvents()
{
	sf::Event e;
	while (window->pollEvent(e))
	{
		if (e.Event::type == sf::Event::Closed)
		{
			window->close();
		}
		if (e.Event::KeyPressed && e.Event::key.code ==
			sf::Keyboard::Escape)
		{
			window->close();
		}
	}
}

void Game::updateInput()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
	{
		this->snake->dir = sf::Vector2f(-1.f, 0.f);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
	{
		this->snake->dir = sf::Vector2f(1.f, 0.f);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
	{
		this->snake->dir = sf::Vector2f(0.f, -1.f);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
	{
		this->snake->dir = sf::Vector2f(0.f, 1.f);
	}
}

void Game::updateSnake()
{
	if (this->dt == 0.f)
	{
		this->snake->move(this->snake->dir);
	}
}

void Game::update()
{
	this->updateDt();
	this->updatePollEvents();
	this->updateInput();
	this->updateSnake();
	this->updateCollsion();
}

void Game::renderSnake()
{
	for (auto& node : this->snake->body)
	{
		this->window->draw(node);
	}
}

void Game::renderMap()
{
	for (int x = 0; x < this->sizeX; x++)
	{
		for (int y = 0; y < this->sizeY; y++)
		{
			this->map[x][y]->render(*this->window);
		}
	}
}

void Game::render()
{
	this->window->clear();

	//render some stuff
	
	this->renderMap();
	this->renderSnake();
	this->renderApple();
	this->window->display();
}

void Game::run()
{
	while (window->isOpen())
	{
		this->updatePollEvents();
		this->update();
		this->render();
	}
}



