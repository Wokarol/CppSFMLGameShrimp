#include <actors\ui\button.h>
#include <gameState.h>

void wok::ui::Button::start(const GameClock&)
{
    float scaleFactor = 1.2f;

    // Creating rectangle
    rectangle = sf::RectangleShape(size);
    rectangle.setOrigin(m::scale(rectangle.getSize(), anchor));

    // Creating label
    label = sf::Text(content, *font, fontSize * scaleFactor);
    label.scale(1.f / scaleFactor, 1.f / scaleFactor);

    auto labelLocalBounds = label.getLocalBounds();
    sf::Vector2f labelSize(labelLocalBounds.width, labelLocalBounds.height);
    sf::Vector2f labelOffset(labelLocalBounds.left, labelLocalBounds.top);
    label.setOrigin(labelSize / 2.f + labelOffset);
}

void wok::ui::Button::draw(sf::RenderTarget& target, sf::RenderStates& states)
{
    sf::Vector2f screenSize = target.getView().getSize();
    sf::Vector2f screenTopLeft = target.getView().getCenter() - screenSize / 2.f;

    rectangle.setPosition(screenTopLeft + m::scale(screenSize, anchor) + offsetFromAnchor);
    rectangle.setFillColor(backgroundColor);

    if (isMouseOver)
    {
        rectangle.move(10, 0);
    }

    label.setPosition(rectangle.getPosition() - rectangle.getOrigin() + rectangle.getSize() / 2.f);

    target.draw(rectangle, states);
    target.draw(label, states);
}

void wok::ui::Button::setOnClick(std::function<void()> onClick)
{
    this->onClick = onClick;
}

void wok::ui::Button::processMouseEvent(MouseEvent& ctx)
{
    if (game::fader.isFading())
        return;

    if (rectangle.getGlobalBounds().contains(ctx.mousePosition))
    {
        isMouseOver = true;

        if (ctx.eventType == MouseEventType::Pressed)
        {
            onClick();
            ctx.consumed = true;
        }
    }
    else
    {
        isMouseOver = false;
    }
}
