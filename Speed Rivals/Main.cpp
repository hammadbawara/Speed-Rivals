#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;

const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 800;
const int TRACK1_WIDTH = 450;

// Generate random number from start to end
int randomInt(int start, int end)
{
    int random;
    random = rand() % (end - start + 1) + start;
    return random;
}



class Sign {
private:
    Sprite signSprite;
    Texture signTexture;
    float yPosition;
public:

    Sign() {

    }
    Sign(float xPosition, float yPosition) : yPosition(yPosition)
    {
        signTexture.loadFromFile("images/road_sign.png");
        signSprite.setTexture(signTexture);
        signSprite.setScale(1, 1);
        signSprite.setPosition(xPosition, yPosition);
    }


    void move(float deltaTime, float speed)
    {
        signSprite.move(0, speed * deltaTime); // Move upwards

        if (signSprite.getPosition().y > WINDOW_HEIGHT + 150)
        {                                          // Check if the road is off-screen
            signSprite.setPosition(signSprite.getPosition().x, -150); // Reset sign position at the bottom
        }
    }

    void draw(RenderWindow& window)
    {
        window.draw(signSprite);
    }
};

class GameEntity {
protected:
    Sprite sprite;
    Texture texture;
    float yPosition;
public:

    GameEntity() {
    }
    GameEntity(float yPosition, int road, Texture& texture) : yPosition(yPosition), texture(texture)
    {
        sprite.setTexture(texture);
        sprite.setScale(1, 1);
        switch (road)
        {
        case 1:
            sprite.setPosition(0, yPosition);
            break;
        case 2:
            sprite.setPosition(160, yPosition);
            break;
        case 3:
            sprite.setPosition(310, yPosition);
            break;
        default:
            break;
        }
    }

    void draw(RenderWindow& window)
    {
        window.draw(sprite);
    }

    void move(float deltaTime, float speed)
    {
        sprite.move(0, speed * deltaTime); // Move upwards
    }
};

class Barrier : public GameEntity {
public:
    Barrier(float yPosition, int road, Texture& texture) : GameEntity(yPosition, road, texture) {}
};

class Boosters : public GameEntity {
public:
    Boosters(float yPosition, int road, Texture& texture) : GameEntity(yPosition, road, texture) {}
};

class Track {
    GameEntity* gameEntities;
    int noOfEntities;
    int totalDistance;

    Sign roadSigns[14]={Sign(140, 0), Sign(140, 150), Sign(140, 300), Sign(140, 450), Sign(140, 600), Sign(140, 750), Sign(140, 900),
                          Sign(290, 0), Sign(290, 150), Sign(290, 300), Sign(290, 450), Sign(290, 600), Sign(290, 750), Sign(290, 900)};

    int road;
    Texture boosterTexture;
    Texture barrierTexture;
public:

    Track(int totalDistance, int r) {
        this->totalDistance = totalDistance;

        road = r;

        noOfEntities = totalDistance / 300;

        gameEntities = new GameEntity[noOfEntities];

        boosterTexture.loadFromFile("./images/barrier.png");
        barrierTexture.loadFromFile("./images/boosters.png");
    }
    void generate()
    {
        for (int i = 0; i < noOfEntities; i++)
        {
            int random_road = randomInt(1, 3);
            int random_entity = randomInt(0, 1);
            if (random_entity == 0)
            {
                gameEntities[i] = Barrier(i * -300, random_road, barrierTexture);
            }
            else if (random_entity == 1)
            {
                gameEntities[i] = Boosters(i * -300, random_road, boosterTexture);
            }
        }
    }

    void draw(RenderWindow& window) {
        for (int j = 0; j < 14; j++)
        {
            roadSigns[j].draw(window);
        }
        for (int i = 0; i < noOfEntities; i++)
        {
            gameEntities[i].draw(window);
        }
    }

    void move(float deltaTime, float speed) {
        for (int j = 0; j < 14; j++)
        {
            roadSigns[j].move(deltaTime, speed);
        }
        for (int i = 0; i < noOfEntities; i++)
        {
            gameEntities[i].move(deltaTime, speed);
        }

    }
};

class Car
{
private:
    Sprite carSprite;
    Vector2f position;
    Texture carTexture;
    float speed;
    float topSpeed;
    float acceleration;
public:
    Car(Vector2f position) : position(position)
    {

        carTexture.loadFromFile("./images/car.png");

        carSprite.setTexture(carTexture);
        carSprite.setPosition(position.x, position.y); // Set initial position at the bottom
        speed = 0;
        topSpeed = 1000;
        acceleration = 0;
    }

    void draw(RenderWindow& window)
    {
        window.draw(carSprite);
    }

    FloatRect getBounds() const
    {
        return carSprite.getGlobalBounds();
    }

    void moveLeft(float deltaTime, float speed)
    {
        float newX = carSprite.getPosition().x - speed * deltaTime;
        if (newX > 0 || newX < TRACK1_WIDTH) {
            carSprite.move(-speed * deltaTime, 0); // Move left
        }

    }

    void moveRight(float deltaTime, float speed)
    {
        float newX = carSprite.getPosition().x + speed * deltaTime;
        if (newX > 0 || newX < TRACK1_WIDTH) {
            carSprite.move(speed * deltaTime, 0); // Move right
        }
    }

    void accelerate(float deltaTime, float s) {
        acceleration += s * deltaTime;
        if (speed < topSpeed) {
            speed += acceleration * deltaTime;
        }
    }

    void decelerate(float deltaTime, float s) {
		acceleration -= s * deltaTime;
        if (speed > 0) {
			speed -= acceleration * deltaTime;
		}
	}

    void move(Track& track, float deltaTime) {
        track.move(speed, deltaTime);
    }
};

int main()
{
    RenderWindow window(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Car Game");

    Car car(Vector2f(180, 600));

    Texture roadTexture;
    roadTexture.loadFromFile("images/road.png");
    Sprite road(roadTexture);
    road.setScale(1, 1);

    Track track1(2000, 1);
    track1.generate();

    Clock clock;

    
    while (window.isOpen())
    {
        bool isAcceleratorPressed = false;
        Event event;
        float deltaTime = clock.restart().asSeconds();

        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

        }

        if (Keyboard::isKeyPressed(Keyboard::Up))
        {
            isAcceleratorPressed = true;
        }

        if (Keyboard::isKeyPressed(Keyboard::Down))
        {
            car.decelerate(deltaTime, 30);
        }
        

        if (Keyboard::isKeyPressed(Keyboard::Left))
        {
            car.moveLeft(deltaTime, 200);
        }

        if (Keyboard::isKeyPressed(Keyboard::Right))
        {
            car.moveRight(deltaTime, 200);
        }

        if (isAcceleratorPressed)
        {
            car.accelerate(deltaTime, 50);
        }
        else
        {
			car.decelerate(deltaTime, 2);
		}

        window.clear();
        window.draw(road);

        track1.draw(window);

        car.move(track1, deltaTime);


        car.draw(window);

        window.display();
    }

    return 0;
}