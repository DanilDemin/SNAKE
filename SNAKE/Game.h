#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

class Game
{
private:
	class Tile
	{
	public:
		Tile(float x,float y, float size)
		{	
			this->tile.setSize(sf::Vector2f(size, size));
			this->tile.setFillColor(sf::Color::Black);
			this->tile.setOutlineThickness(1.f);
			this->tile.setOutlineColor(sf::Color(128, 128, 128));
			this->tile.setPosition(x, y);
		}
		virtual ~Tile(){}
		sf::RectangleShape tile;

		void update(){}
		void render(sf::RenderTarget& target)
		{
			target.draw(this->tile);
		}

	};

	class Snake 
	{
	public:
		std::vector<sf::RectangleShape> body;
		sf::Vector2f dir;
		unsigned int size_itself;
		float size;

		Snake(const float size)
		{
			this->body = std::vector<sf::RectangleShape>(0);
			this->size_itself = 0;
			this->size = size;
			this->dir = sf::Vector2f(0.f, 0.f);

			for (int i = 0; i < 5; i++)
			{
				sf::RectangleShape node;
				node.setPosition(0.f, static_cast<float>(i) * size);
				node.setFillColor(sf::Color(139, 0, 0));
				node.setSize(sf::Vector2f(size, size));
				this->body.push_back(node);
				this->size_itself++;
			}
		}
		virtual ~Snake(){}

		void move(sf::Vector2f dir)
		{
			//move body 
			for (int index_node = this->size_itself - 1; index_node > 0; index_node--)
			{
				this->body[index_node].setPosition(this->body[index_node - 1].getPosition());
			}
			//move head
			this->body[0].setPosition(
				this->body[0].getPosition().x + dir.x * this->size,
				this->body[0].getPosition().y + dir.y * this->size
			);
		}

	};

	sf::RenderWindow* window;
	std::vector<std::vector<Tile*>> map;
	Snake* snake;
	sf::RectangleShape* apple;

	float dt;
	float maxDt;
	float size;
	int sizeX;
	int sizeY;


	void initMap();
	void initSnake();
	void deleteMap();

public:
	Game();
	virtual ~Game();
	

	//Functions
	void SpawnApple();


	//update
	void updateDt();
	void updatePollEvents();
	void updateInput();
	void updateSnake();
	void updateCollsion();
	void update();

	

	//render
	void renderApple();
	void renderSnake();
	void renderMap();
	void render();

	//run
	void run();
};

