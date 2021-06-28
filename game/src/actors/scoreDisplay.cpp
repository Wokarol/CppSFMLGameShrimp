#include <actors/scoreDisplay.h>

#include <resources.h>
#include <gameState.h>

#include <sstream>

wok::ScoreDisplay::ScoreDisplay()
{
    font = res::get<sf::Font>("RobotoMono");

    float scaleFactor = 1.5f;

    text.setFont(*font);

    text.setCharacterSize(8u * scaleFactor);
    text.scale(1.f / scaleFactor, 1.f / scaleFactor);
    text.setColor(sf::Color::White);
    text.setOutlineColor(sf::Color(128, 128, 128));
    text.setOutlineThickness(1);
}

void wok::ScoreDisplay::update(const GameClock& time)
{
    sf::Vector2f startPos = game::getCurrentCamera().viewportToWorld({ 0.f, 0.f });
    sf::Vector2f textOffset(10, 25);

    text.setPosition(startPos + textOffset);

    std::stringstream scoreMessage;
    scoreMessage << "Score: " << game::score.getPoints();

    text.setString(scoreMessage.str());
}

void wok::ScoreDisplay::draw(sf::RenderTarget& target, sf::RenderStates& states)
{
    target.draw(text, states);
}
