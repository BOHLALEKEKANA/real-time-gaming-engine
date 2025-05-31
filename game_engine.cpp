#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

// Entity class to represent game objects
class Entity {
public:
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::CircleShape shape;

    Entity(float x, float y, float radius) : position(x, y), velocity(0.f, 0.f) {
        shape.setRadius(radius);
        shape.setPosition(position);
        shape.setFillColor(sf::Color::Green);
    }

    void update(float dt) {
        position += velocity * dt;
        shape.setPosition(position);
    }
};

// Game Engine class
class GameEngine {
private:
    sf::RenderWindow window;
    std::vector<std::unique_ptr<Entity>> entities;
    const float moveSpeed = 200.f; // Pixels per second

public:
    GameEngine(unsigned int width, unsigned int height, const std::string& title)
        : window(sf::VideoMode(width, height), title) {
        window.setFramerateLimit(60);
    }

    void addEntity(float x, float y, float radius) {
        entities.push_back(std::make_unique<Entity>(x, y, radius));
    }

    void handleInput() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // Handle continuous input for movement
        if (entities.empty()) return;
        auto& player = entities[0]; // Assume first entity is player
        player->velocity = {0.f, 0.f};
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            player->velocity.y = -moveSpeed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            player->velocity.y = moveSpeed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            player->velocity.x = -moveSpeed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            player->velocity.x = moveSpeed;
        }
    }

    void update(float dt) {
        for (auto& entity : entities) {
            entity->update(dt);
        }
    }

    void render() {
        window.clear(sf::Color::Black);
        for (auto& entity : entities) {
            window.draw(entity->shape);
        }
        window.display();
    }

    void run() {
        sf::Clock clock;
        const float dt = 1.f / 60.f; // Fixed time step (60 FPS)

        while (window.isOpen()) {
            handleInput();
            update(dt);
            render();
        }
    }
};

int main() {
    GameEngine engine(800, 600, "Simple Game Engine");
    engine.addEntity(400.f, 300.f, 20.f); // Add a player entity
    engine.addEntity(200.f, 200.f, 15.f); // Add another entity
    engine.run();
    return 0;
}