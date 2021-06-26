#include <actors\ui\button.h>

void wok::ui::Button::draw(sf::RenderTarget& target, sf::RenderStates& states)
{
    sf::RectangleShape rectangle(size);

    sf::Vector2f screenSize = target.getView().getSize();
    sf::Vector2f screenTopLeft = target.getView().getCenter() - screenSize / 2.f;

    rectangle.setOrigin(m::scale(rectangle.getSize(), anchor));
    rectangle.setPosition(screenTopLeft + m::scale(screenSize, anchor) + offsetFromAnchor);
    rectangle.setFillColor(backgroundColor);


    float scaleFactor = 1.2f;
    sf::Text label(content, *font, fontSize * scaleFactor);
    label.scale(1.f / scaleFactor, 1.f / scaleFactor);

    auto labelLocalBounds = label.getLocalBounds();
    sf::Vector2f labelSize(labelLocalBounds.width, labelLocalBounds.height);
    sf::Vector2f labelOffset(labelLocalBounds.left, labelLocalBounds.top);

    label.setOrigin(labelSize / 2.f + labelOffset);
    label.setPosition(rectangle.getPosition() - rectangle.getOrigin() + rectangle.getSize() / 2.f);

    target.draw(rectangle, states);
    target.draw(label, states);
}
