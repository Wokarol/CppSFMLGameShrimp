#include <actors\ui\title.h>

void wok::ui::Title::draw(sf::RenderTarget& target, sf::RenderStates& states)
{
    float scaleFactor = 1.2f;

    sf::Text text(title, *font, (unsigned int)(characterSize * scaleFactor));
    text.scale(1.f / scaleFactor, 1.f / scaleFactor);
    sf::FloatRect size = text.getLocalBounds();


    text.setOrigin(size.width / 2.f, 0.f);
    text.setFillColor(fillColor);

    sf::Vector2f topCentreOfScreen = target.mapPixelToCoords({ (int)target.getSize().x / 2, 0 });
    text.setPosition(topCentreOfScreen + sf::Vector2f(0.f, positionFromTop));

    target.draw(text, states);
}
