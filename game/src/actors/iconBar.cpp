#include <actors/iconBar.h>

#include <resources.h>
#include <gameState.h>

wok::IconBar::IconBar(std::shared_ptr<IconBarSettings> settings, int maxValue) :
    settings(settings), maxValue(maxValue)
{
    texture = res::get<sf::Texture>(settings->iconPath);
    currentValue = maxValue;

    for (int i = 0; i < maxValue; i++)
    {
        animators.emplace_back(
            settings->animationFramerate,
            settings->spriteRectFull, settings->spriteRectEmpty,
            settings->animationStripGained, settings->animationStripLost,
            true
        );
    }
    frames = std::vector<sf::Vector2i>(maxValue);
}

void wok::IconBar::setValue(int value)
{
    currentValue = value;
}

void wok::IconBar::update(const GameClock& time)
{
    for (int i = 0; i < maxValue; i++)
    {
        frames[i] = animators[i].update(time.delta, currentValue > i);
    }
}

void wok::IconBar::draw(sf::RenderTarget& target, sf::RenderStates& states)
{
    sf::Vector2f iconOffset = sf::Vector2f(
        (float)settings->iconSize.x + settings->spacing,
        0
    );

    for (int i = 0; i < maxValue; i++)
    {
        sf::Vector2i iconPosOnTexture = frames[i];

        sf::IntRect rect(
            iconPosOnTexture,
            settings->iconSize
        );
        sf::Sprite healthIcon(*texture, rect);

        sf::Vector2f startPos = game::getCurrentCamera().viewportToWorld({ 0.f, 0.f });
        healthIcon.setPosition(startPos + settings->offset + iconOffset * (float)i);
        target.draw(healthIcon, states);
    }
}
