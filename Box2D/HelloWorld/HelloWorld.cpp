#include <SFML\Graphics.hpp>
#include <Box2D\Box2D.h>
#include <iostream>

#ifdef _DEBUG 
#pragma comment(lib,"sfml-graphics-d.lib") 
#pragma comment(lib,"sfml-audio-d.lib") 
#pragma comment(lib,"sfml-system-d.lib") 
#pragma comment(lib,"sfml-window-d.lib") 
#pragma comment(lib,"sfml-network-d.lib") 
#else 
#pragma comment(lib,"sfml-graphics.lib") 
#pragma comment(lib,"sfml-audio.lib") 
#pragma comment(lib,"sfml-system.lib") 
#pragma comment(lib,"sfml-window.lib") 
#pragma comment(lib,"sfml-network.lib") 
#endif 

/** We need this to easily convert between pixel and real-world coordinates*/
static const float SCALE = 30.f;

/** Create the base for the boxes to land */
void CreateGround(b2World& World, float X, float Y);

/** Create the boxes */
void CreateBox(b2World& World, int MouseX, int MouseY);

void player(b2World& World,b2BodyDef & BodyDef);

int main()
{
	/** Prepare the window */
	sf::RenderWindow m_Window(sf::VideoMode(800, 600, 32), "Box2D and SFML");
	m_Window.setFramerateLimit(60);

	/** Prepare the world */
	b2Vec2 Gravity(0.f, 9.8f);
	b2World World(Gravity);
	CreateGround(World, 400.f, 500.f);

	/** Prepare textures */
	sf::Texture GroundTexture;
	sf::Texture BoxTexture;
	GroundTexture.loadFromFile("ground.png");
	BoxTexture.loadFromFile("box.png");

	sf::Sprite PlayerSprite;
	PlayerSprite.setTexture(BoxTexture);
	PlayerSprite.setOrigin(16.f, 16.f);

	b2BodyDef m_playerBox;
	m_playerBox.position = b2Vec2(400.0f / SCALE, 300.0f / SCALE);
	m_playerBox.type = b2_dynamicBody;
	b2Body* Body = World.CreateBody(&m_playerBox);

	b2PolygonShape Shape;
	Shape.SetAsBox((32.f / 2) / SCALE, (32.f / 2) / SCALE);

	b2FixtureDef FixtureDef;
	FixtureDef.density = 1.f;
	FixtureDef.friction = 0.7f;
	FixtureDef.shape = &Shape;
	Body->CreateFixture(&FixtureDef);
	

	//player(World, m_playerBox);

	while (m_Window.isOpen())
	{

		PlayerSprite.setPosition(m_playerBox.position.x, m_playerBox.position.y);
		
		World.Step(1 / 60.f, 8, 3);

		m_Window.clear(sf::Color::White);

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			//m_playerBox.position = b2Vec2(sf::Mouse::getPosition(m_Window).x, sf::Mouse::getPosition(m_Window).y);

			//CreateBox(World, sf::Mouse::getPosition(m_Window).x, sf::Mouse::getPosition(m_Window).y);
			m_playerBox.linearVelocity = b2Vec2(sf::Mouse::getPosition(m_Window).x - m_playerBox.position.x, sf::Mouse::getPosition(m_Window).y - m_playerBox.position.y);

			
		}

		int BodyCount = 0;

		for (b2Body* BodyIterator = World.GetBodyList(); BodyIterator != 0; BodyIterator = BodyIterator->GetNext())
		{
			if (BodyIterator->GetType() == b2_dynamicBody)
			{
				sf::Sprite Sprite;
				Sprite.setTexture(BoxTexture);
				Sprite.setOrigin(16.f, 16.f);
				Sprite.setPosition(SCALE * BodyIterator->GetPosition().x, SCALE * BodyIterator->GetPosition().y);
				Sprite.setRotation(BodyIterator->GetAngle() * 180 / b2_pi);
				m_Window.draw(Sprite);
				++BodyCount;
			}
			else
			{
				sf::Sprite GroundSprite;
				GroundSprite.setTexture(GroundTexture);
				GroundSprite.setOrigin(400.f, 8.f);
				GroundSprite.setPosition(BodyIterator->GetPosition().x * SCALE, BodyIterator->GetPosition().y * SCALE);
				GroundSprite.setRotation(180 / b2_pi * BodyIterator->GetAngle());
				m_Window.draw(GroundSprite);
			}
		}


		m_Window.draw(PlayerSprite);
		m_Window.display();
	}

	return 0;
}

void CreateBox(b2World& World, int MouseX, int MouseY)
{
	b2BodyDef BodyDef;
	BodyDef.position = b2Vec2(MouseX / SCALE, MouseY / SCALE);
	BodyDef.type = b2_dynamicBody;
	b2Body* Body = World.CreateBody(&BodyDef);

	b2PolygonShape Shape;
	Shape.SetAsBox((32.f / 2) / SCALE, (32.f / 2) / SCALE);

	b2FixtureDef FixtureDef;
	FixtureDef.density = 1.f;
	FixtureDef.friction = 0.7f;
	FixtureDef.shape = &Shape;
	Body->CreateFixture(&FixtureDef);
}

void player(b2World& World, b2BodyDef & BodyDef)
{
		BodyDef.position = b2Vec2(100 / SCALE, 100 / SCALE);
		BodyDef.type = b2_dynamicBody;

		BodyDef.linearVelocity = b2Vec2(5, 5);

		b2Body* dynamicBody = World.CreateBody(&BodyDef);
		
		b2PolygonShape boxShape;
		boxShape.SetAsBox((32.f / 2) / SCALE, (32.f / 2) / SCALE);

		b2FixtureDef boxFixtureDef;
		boxFixtureDef.shape = &boxShape;
		boxFixtureDef.density = 1.f;
		dynamicBody->CreateFixture(&boxFixtureDef);
}


void CreateGround(b2World& World, float X, float Y)
{
	b2BodyDef BodyDef;
	BodyDef.position = b2Vec2(X / SCALE, Y / SCALE);
	BodyDef.type = b2_staticBody;
	b2Body* Body = World.CreateBody(&BodyDef);

	b2PolygonShape Shape;
	Shape.SetAsBox((800.f / 2) / SCALE, (16.f / 2) / SCALE);
	b2FixtureDef FixtureDef;
	FixtureDef.density = 0.f;
	FixtureDef.shape = &Shape;
	Body->CreateFixture(&FixtureDef);
}