#include <SFML/Graphics.hpp>
#include <optional>
#include <cmath>

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "Coyote Time and Jump Buffering");

    sf::RectangleShape square({ 50.f, 50.f });
    square.setFillColor(sf::Color::Green);
    square.setPosition({ 375.f, 300.f });

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

        // Friction
        velocity.x *= 1.f / (1.f + friction * dt);

        // Clamp speed
        if (std::abs(velocity.x) > maxSpeed)
            velocity.x = (velocity.x > 0.f ? 1.f : -1.f) * maxSpeed;

        // Timers
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

        square.move(velocity * dt);

        sf::Vector2f pos = square.getPosition();
        sf::Vector2f size = square.getSize();

        float groundY = ground.getPosition().y;

        if (pos.y + size.y >= groundY)
        {
            square.setPosition({ pos.x, groundY - size.y });
            velocity.y = 0.f;
            isOnGround = true;
        }
        else
        {
            isOnGround = false;
        }

        // Screen walls
        pos = square.getPosition();

        if (pos.x < 0.f)
        {
            square.setPosition({ 0.f, pos.y });
            velocity.x = 0.f;
        }

        if (pos.x + size.x > 800.f)
        {
            square.setPosition({ 800.f - size.x, pos.y });
            velocity.x = 0.f;
        }

        window.clear();
        window.draw(ground);
        window.draw(square);
        window.display();
    }

    return 0;
}