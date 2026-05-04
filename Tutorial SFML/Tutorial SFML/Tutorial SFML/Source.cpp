#include <SFML/Graphics.hpp>
#include <optional>
#include <cmath>
#include <vector>

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "SFML Test");

    sf::Texture duckTexture;
    if (!duckTexture.loadFromFile("assets/grey_duck_movement.png"))
        return -1;

    const int frameWidth = 15;
    const int frameHeight = 25;
    const float scale = 4.f;

    sf::Sprite player(duckTexture);

    std::vector<sf::IntRect> walkFrames =
    {
        sf::IntRect({ 0, 0 },   { frameWidth, frameHeight }),
        sf::IntRect({ 32, 0 },  { frameWidth, frameHeight }),
        sf::IntRect({ 64, 0 },  { frameWidth, frameHeight }),
        sf::IntRect({ 96, 0 },  { frameWidth, frameHeight }),
        sf::IntRect({ 128, 0 }, { frameWidth, frameHeight }),
        sf::IntRect({ 160, 0 }, { frameWidth, frameHeight })
    };

    player.setTextureRect(walkFrames[0]);
    player.setOrigin({ frameWidth / 2.f, frameHeight / 2.f });
    player.setPosition({ 400.f, 300.f });
    player.setScale({ scale, scale });

    sf::RectangleShape ground({ 800.f, 50.f });
    ground.setFillColor(sf::Color(100, 100, 100));
    ground.setPosition({ 0.f, 550.f });

    sf::Vector2f velocity(0.f, 0.f);

    const float acceleration = 1200.f;
    const float maxSpeed = 300.f;
    const float friction = 8.f;

    const float gravity = 2000.f;
    const float jumpForce = -700.f;

    const float coyoteTime = 0.12f;
    const float jumpBufferTime = 0.12f;

    float coyoteTimer = 0.f;
    float jumpBufferTimer = 0.f;

    bool isOnGround = false;

    float animationTimer = 0.f;
    const float animationSpeed = 0.08f;
    int currentFrame = 0;

    bool facingRight = true;

    sf::Clock clock;

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Space)
                    jumpBufferTimer = jumpBufferTime;
            }
        }

        float dt = clock.restart().asSeconds();

        float inputX = 0.f;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
            inputX -= 1.f;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
            inputX += 1.f;

        velocity.x += inputX * acceleration * dt;
        velocity.x *= 1.f / (1.f + friction * dt);

        if (std::abs(velocity.x) > maxSpeed)
            velocity.x = (velocity.x > 0.f ? 1.f : -1.f) * maxSpeed;

        if (isOnGround)
            coyoteTimer = coyoteTime;
        else
            coyoteTimer -= dt;

        jumpBufferTimer -= dt;

        if (jumpBufferTimer > 0.f && coyoteTimer > 0.f)
        {
            velocity.y = jumpForce;
            jumpBufferTimer = 0.f;
            coyoteTimer = 0.f;
            isOnGround = false;
        }

        velocity.y += gravity * dt;

        player.move(velocity * dt);

        sf::Vector2f pos = player.getPosition();

        float duckWidth = frameWidth * scale;
        float duckHeight = frameHeight * scale;

        float groundY = ground.getPosition().y;

        if (pos.y + duckHeight / 2.f >= groundY)
        {
            player.setPosition({ pos.x, groundY - duckHeight / 2.f });
            velocity.y = 0.f;
            isOnGround = true;
        }
        else
        {
            isOnGround = false;
        }

        pos = player.getPosition();

        if (pos.x - duckWidth / 2.f < 0.f)
        {
            player.setPosition({ duckWidth / 2.f, pos.y });
            velocity.x = 0.f;
        }

        if (pos.x + duckWidth / 2.f > 800.f)
        {
            player.setPosition({ 800.f - duckWidth / 2.f, pos.y });
            velocity.x = 0.f;
        }

        bool isMoving = std::abs(velocity.x) > 20.f;

        if (velocity.x > 5.f)
            facingRight = true;
        else if (velocity.x < -5.f)
            facingRight = false;

        if (isMoving)
        {
            animationTimer += dt;

            if (animationTimer >= animationSpeed)
            {
                animationTimer = 0.f;
                currentFrame = (currentFrame + 1) % walkFrames.size();
                player.setTextureRect(walkFrames[currentFrame]);
            }
        }
        else
        {
            currentFrame = 0;
            player.setTextureRect(walkFrames[0]);
        }

        if (facingRight)
            player.setScale({ scale, scale });
        else
            player.setScale({ -scale, scale });

        window.clear(sf::Color::Black);
        window.draw(ground);
        window.draw(player);
        window.display();
    }

    return 0;
}