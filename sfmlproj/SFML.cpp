#include <iostream>
#include <random>
#include <chrono>
#include <SFML/Graphics.hpp>
#include <string>
#include <sstream>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>

sf::Color HSVToRGB(float h, float s, float v) {
	float hPrime = h / 60.0f;
	unsigned int hIndex = unsigned int(hPrime) % 6;
	float chroma = s * v;
	float min = (v - chroma);
	float x = chroma * (1.0f - abs(fmod(hPrime, 2.0f) - 1.0f));
	float outRGB[6][3] = {

		{chroma, x, 0.0f},
		{x, chroma, 0.0f},
		{0.0f, chroma, x},
		{0.0f, x, chroma},
		{x, 0.0f, chroma},
		{chroma, 0.0f, x}

	};
	float rF = (outRGB[hIndex][0] + min);
	float gF = (outRGB[hIndex][1] + min);
	float bF = (outRGB[hIndex][2] + min);
	rF *= 255;
	gF *= 255;
	bF *= 255;
	std::uint8_t rI = std::uint8_t(rF);
	std::uint8_t gI = std::uint8_t(gF);
	std::uint8_t bI = std::uint8_t(bF);
	return sf::Color(rI, gI, bI);
}
void PollEvents(sf::RenderWindow& window) {
	while (const std::optional event = window.pollEvent()) {
		if (event->is<sf::Event::Closed>()) {
			window.close();
		}
		else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
			if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
				window.close();
			}
		}
	}
}

int main() {
	unsigned int width = 640;
	unsigned int height = 320;
	sf::Vector2u windowSize = { width, height };
	sf::VideoMode videomode = sf::VideoMode(windowSize);
	std::string title = "SFMLL";
	sf::RenderWindow window = sf::RenderWindow(sf::VideoMode({ width, height }), title);
	window.setFramerateLimit(60);

	std::default_random_engine ranengine;
	ranengine.seed(std::chrono::steady_clock::now().time_since_epoch().count());

	std::uniform_real_distribution<float> ranSpeed;
	ranSpeed = std::uniform_real_distribution<float>(-4.0f, 4.0f);



	sf::Vector2f ballspeed = { 5.0f, 5.0f };

	sf::RectangleShape airect;
	sf::Vector2f paddleSize = { 80.0f, 10.0f };
	airect.setSize(paddleSize);
	airect.setFillColor(sf::Color::Red);
	airect.setOutlineThickness(2.0f);
	airect.setOutlineColor(sf::Color::Cyan);
	airect.setPosition({ width / 2.0f, height - 305.0f });

	sf::RectangleShape hurect;
	hurect.setSize(paddleSize);
	hurect.setFillColor(sf::Color::Cyan);
	hurect.setOutlineThickness(2.0f);
	hurect.setOutlineColor(sf::Color::Red);
	hurect.setPosition({ width / 2.0f, height - 30.0f });

	float bRadius = 8.0f;
	sf::CircleShape ball(bRadius);
	ball.setFillColor(sf::Color::Yellow);
	ball.setOutlineThickness(1.0f);
	ball.setOutlineColor(sf::Color::Green);
	ball.setPosition({ width / 2.0f, height / 2.0f });

	int playerScore = 0;
	int aiScore = 0;

	while (window.isOpen()) {


		//Updating

		PollEvents(window);

		ball.move(ballspeed);


		if (ball.getGlobalBounds().findIntersection(hurect.getGlobalBounds()) && ballspeed.y > 0) {

			float hitOffset = (ball.getPosition().x + bRadius) - (hurect.getPosition().x + paddleSize.x / 2.0f);
			ballspeed.x = hitOffset * 0.13f;
			ballspeed.y = -std::abs(ballspeed.y) * 1.0f;

		}
		if (ball.getGlobalBounds().findIntersection(airect.getGlobalBounds()) && ballspeed.y < 0) {

			float hitOffset = (ball.getPosition().x + bRadius) - (airect.getPosition().x + paddleSize.x / 2.0f);
			ballspeed.x = hitOffset * 0.13f;
			ballspeed.y = std::abs(ballspeed.y) * 1.0f;

		}
		if (ball.getPosition().y < 0) {
			playerScore++;
			std::cout << "Player: " << playerScore << " | Bot: " << aiScore << "\n";

			ball.setPosition({ width / 2.0f - bRadius, height / 2.0f - bRadius });
			ballspeed = { 0.0f, 4.0f };
		}

		if (ball.getPosition().y > height) {
			aiScore++;
			std::cout << "Player: " << playerScore << " | Bot: " << aiScore << "\n";

			ball.setPosition({ width / 2.0f - bRadius, height / 2.0f - bRadius });
			ballspeed = { 0.0f, -4.0f };
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A)) {

			hurect.move({ -2.0f, 0.0f });

		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D)) {

			hurect.move({ 2.0f, 0.0f });

		}
		

		airect.move({ ranSpeed(ranengine), 0.0f});


		if (ball.getPosition().x <= 0 || ball.getPosition().x + bRadius * 2.0f >= width) {

			ballspeed.x = -ballspeed.x;

		}



		//Render

		window.clear(sf::Color::Black);

		// Draw
		
		window.draw(airect);
		window.draw(hurect);
		window.draw(ball);
		
		// Display screen

		window.display();
	}
	return 0;
}