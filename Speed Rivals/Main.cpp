#include <SFML/Graphics.hpp>
using namespace sf;

// Constants for window dimensions and track width
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

// Class definition for road signs
class Sign {
private:
    Sprite signSprite;
    Texture signTexture;
    float yPosition;
public:

    Sign() {

    }

    void setSignValues(float xPosition, float yPosition) {
        this->yPosition = yPosition;
        signTexture.loadFromFile("images/road_sign.png");
        signSprite.setTexture(signTexture);
        signSprite.setPosition(xPosition, yPosition);
    }

    void move(float deltaTime, float speed)
    {
        signSprite.move(0, speed * deltaTime); // Move upwards

        if (signSprite.getPosition().y > WINDOW_HEIGHT + 200)
        {                                          // Check if the road is off-screen
            signSprite.setPosition(signSprite.getPosition().x, -200); // Reset sign position at the bottom
        }
    }

    void draw(RenderWindow& window)
    {
        window.draw(signSprite);
    }
};

// Class definition for game entities
class GameEntity {
protected:
    Sprite sprite;
    Texture texture;
    float yPosition;
public:

    GameEntity() {
    }
    GameEntity(float yPosition, int road, Texture& texture, int track) : yPosition(yPosition), texture(texture)
    {
        sprite.setTexture(texture);
        sprite.setScale(1, 1);

        if (track == 1) {
            switch (road)
            {
            case 1:
                sprite.setPosition(10, yPosition);
                break;
            case 2:
                sprite.setPosition(170, yPosition);
                break;
            case 3:
                sprite.setPosition(325, yPosition);
                break;
            default:
                break;
            }
        }
        else if (track == 2) {
            switch (road)
            {
            case 1:
                sprite.setPosition(550, yPosition);
                break;
            case 2:
                sprite.setPosition(710, yPosition);
                break;
            case 3:
                sprite.setPosition(860, yPosition);
                break;
            default:
                break;
            }
        }
        
        
    }

    Sprite& getSprite() {
        return sprite;
    }

    void draw(RenderWindow& window)
    {
        window.draw(sprite);
    }

    FloatRect getBounds() const
    {
		return sprite.getGlobalBounds();
	}

    void move(float deltaTime, float speed)
    {
        sprite.move(0, speed * deltaTime); // Move upwards
    }

    virtual std::string getType() {
        return "gameEntity";
    }
};

// Class definition for barriers
class Barrier : public GameEntity {
public:
    Barrier(float yPosition, int road, Texture& texture, int track) : GameEntity(yPosition, road, texture, track) {}
    std::string getType() {
        return "barrier";
    }
};

class Boosters : public GameEntity {
public:
    Boosters(float yPosition, int road, Texture& texture, int track) : GameEntity(yPosition, road, texture, track) {}
    std::string getType() {
        return "booster";
    }
};

class Track {
    GameEntity* gameEntities;
    int noOfEntities;
    int totalDistance;

    float currentDistance;

    Sign roadSigns[12];
   
    int track;
    Texture boosterTexture;
    Texture barrierTexture;

    Vector2i boundary;

    Font font;
    Text distanceText;
public:

    Track(int totalDistance, int t, Vector2i boundary) {
        currentDistance = 0;

        this->totalDistance = totalDistance;
        this->boundary = boundary;

        track = t;

        font.loadFromFile("./fonts/Roboto-Regular.ttf");
        distanceText.setFont(font);
        distanceText.setFillColor(Color::Blue);
        distanceText.setPosition(boundary.x + 350, 10);

        noOfEntities = totalDistance / 500;
        gameEntities = new GameEntity[noOfEntities];

        boosterTexture.loadFromFile("./images/barrier.png");
        barrierTexture.loadFromFile("./images/boosters.png");

        // Set road signs
        if (track == 1) {
            for (int i = 0; i < 6; i++) {
                roadSigns[i].setSignValues(140, i * 200);
                roadSigns[i + 6].setSignValues(290, i * 200);
            }
        }
        else if (track == 2) {
            for (int i = 0; i < 6; i++) {
                roadSigns[i].setSignValues(690, i * 200);
                roadSigns[i + 6].setSignValues(840, i * 200);
            }
        }
    }
    void generate()
    {
        for (int i = 0; i < noOfEntities; i++)
        {
            int random_road = randomInt(1, 3);
            int random_entity = randomInt(0, 1);
            if (random_entity == 0)
            {
                gameEntities[i] = Barrier(i * -500, random_road, barrierTexture, track);
            }
            else if (random_entity == 1)
            {
                gameEntities[i] = Boosters(i * -500, random_road, boosterTexture, track);
            }
        }
    }

    int getTotalDistance() {
        return totalDistance;
    }

    void copyEntityInto(Track& track, int xMove) {
        track.noOfEntities = noOfEntities;
        track.gameEntities = new GameEntity[noOfEntities];

        for (int i = 0; i < noOfEntities; i++) {
            track.gameEntities[i] = gameEntities[i];
            track.gameEntities[i].getSprite().setPosition(gameEntities[i].getSprite().getPosition().x + xMove, gameEntities[i].getSprite().getPosition().y);
        }
    }

    void draw(RenderWindow& window) {
        int distance = currentDistance / 5;

        distanceText.setString(std::to_string(distance));

        for (int j = 0; j < 12; j++)
        {
            roadSigns[j].draw(window);
        }

        int startEntity = (currentDistance - 500) / 500;
        if (startEntity < 0) {
            startEntity = 0;
        }
        for (int i = startEntity; i < startEntity + 4; i++)
        {
            if (i >= noOfEntities) {
				break;
			}
            gameEntities[i].draw(window);
        }

        window.draw(distanceText);
    }

    void move(float deltaTime, float speed) {

        currentDistance += speed * deltaTime;

        for (int j = 0; j < 12; j++)
        {
            roadSigns[j].move(deltaTime, speed);
        }
        for (int i = 0; i < noOfEntities; i++)
        {
            gameEntities[i].move(deltaTime, speed);
        }

    }

    Vector2i& getBoundary(){
        return boundary;
    }

    int getNoOfEntities() {
		return noOfEntities;
	}

    GameEntity* getGameEntitity(int i) {
        return &gameEntities[i];
    }

    float& getCurrentDistance() {
        return currentDistance;
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
    Text speedText;
    Font font;
    
public:
    Car(Vector2f position, std::string texturePath) : position(position)
    {
        font.loadFromFile("./fonts/Roboto-Regular.ttf");
        speedText.setFont(font);
        speedText.setPosition(0, 0);
        speedText.setFillColor(Color::Red);

        carTexture.loadFromFile(texturePath);
        carSprite.setTexture(carTexture);
        carSprite.setPosition(position.x, position.y); // Set initial position at the bottom

        speed = 0;
        topSpeed = 1000;
        acceleration = 0;
    }

    void setSpeedTextPosition(Vector2f position) {
        speedText.setPosition(position.x, position.y);
    }

    void draw(RenderWindow& window)
    {
        int speed = (int)this->speed/5;
        speedText.setString(std::to_string(speed));
        
        window.draw(speedText);
        window.draw(carSprite);
    }

    FloatRect getBounds() const
    {
        return carSprite.getGlobalBounds();
    }

    void moveLeft(float deltaTime, float speed, Track& track)
    {
        float newX = carSprite.getPosition().x - (speed * deltaTime);
        if (newX > track.getBoundary().x) {
            carSprite.move(-speed * deltaTime, 0); // Move left
        }

    }

    void moveRight(float deltaTime, float speed, Track& track)
    {
        float newX = carSprite.getPosition().x + speed * deltaTime;
        if (newX < track.getBoundary().x + 360) {
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
        else {
			speed = 0;
		}
	}

    /*void collision(Track& track) {
        for (int i = 0; i < track.getNoOfEntities(); i++) {
            if (getBounds().intersects(track.getGameEntitity(i)->getBounds())) {
                if (track.getGameEntitity(i)->getType() == "barrier") {
					speed = 0;
				}
                else if (track.getGameEntitity(i)->getType() == "booster") {
					speed += 100;
				}
			}
		}
	}*/

    void drawCarOnTrack(Track& currentTrack, Track& otherTrack, RenderWindow& window) {
		float difference = currentTrack.getCurrentDistance() - otherTrack.getCurrentDistance();
        if (difference > - 300 && difference < 700) {
			Car car = *this;
            car.carSprite.move(otherTrack.getBoundary().x - currentTrack.getBoundary().x, -difference);
			car.draw(window);
		}
	}

    void move(Track& track, float deltaTime) {
        track.move(deltaTime, speed);
    }
};

int main()
{
    // Create the game window
    RenderWindow window(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Car Game");

    // Create car objects and set their initial positions
    Car car1(Vector2f(30, 520), "./images/car1.png");
    car1.setSpeedTextPosition(Vector2f(10, 10));

    Car car2(Vector2f(880, 520), "./images/car2.png");
    car2.setSpeedTextPosition(Vector2f(560, 10));

    // Load background texture and create background sprite
    Texture bgTexture;
    bgTexture.loadFromFile("images/road.png");
    Sprite bgSprite(bgTexture);

    // Create track objects and generate game entities
    Track track1(50000, 1, Vector2i(0, 450));
    track1.generate();

    Track track2(50000, 2, Vector2i(550, 900));
    track1.copyEntityInto(track2, 550);

    // Set up clock and window properties
    Clock clock;
    window.setFramerateLimit(60);
    
    while (window.isOpen())
    {
        // Calculate time elapsed since last frame
        float deltaTime = clock.restart().asSeconds();

        bool car2AcceleratorPressed = false;
        bool car1AcceleratorPressed = false;

        // Handle events
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

        }

        // Car 1 Controls
        if (Keyboard::isKeyPressed(Keyboard::W))
        {
            car1AcceleratorPressed = true;
        }
        if (Keyboard::isKeyPressed(Keyboard::S))
        {
            car1.decelerate(deltaTime, 30);
        }
        if (Keyboard::isKeyPressed(Keyboard::A))
        {
            car1.moveLeft(deltaTime, 200, track1);
        }
        if (Keyboard::isKeyPressed(Keyboard::D))
        {
            car1.moveRight(deltaTime, 200, track1);
        }

        if (car1AcceleratorPressed)
        {
            car1.accelerate(deltaTime, 50);
        }
        else
        {
            car1.decelerate(deltaTime, 0.2);
        }

        // Car 2 Controls
        if (Keyboard::isKeyPressed(Keyboard::Up))
        {
            car2AcceleratorPressed = true;
        }

        if (Keyboard::isKeyPressed(Keyboard::Down))
        {
            car2.decelerate(deltaTime, 20);
        }

        if (Keyboard::isKeyPressed(Keyboard::Left))
        {
            car2.moveLeft(deltaTime, 200, track2);
        }

        if (Keyboard::isKeyPressed(Keyboard::Right))
        {
            car2.moveRight(deltaTime, 200, track2);
        }

        if (car2AcceleratorPressed)
        {
            car2.accelerate(deltaTime, 50);
        }
        else
        {
			car2.decelerate(deltaTime, 0.2);
		}
        
        window.clear();

        // Drawing the game entities
        window.draw(bgSprite);
        track1.draw(window);
        track2.draw(window);

        // Moving and drawing the cars
        car1.move(track1, deltaTime);
        car2.move(track2, deltaTime);

        car1.draw(window);
        car2.draw(window);

        // Drawing car on other track
        car1.drawCarOnTrack(track1, track2, window);
        car2.drawCarOnTrack(track2, track1, window);

        // Check if the car has reached the end of the track
        if (track1.getCurrentDistance() > track1.getTotalDistance()) {
            window.close();
        }
        if (track2.getCurrentDistance() > track1.getTotalDistance()) {
            window.close();
        }

        // Display the updated window
        window.display();
    }

    return 0;
}