#pragma once

#include <actor.h>
#include <resources.h>
#include <assets/iconBarSettings.h>

namespace wok
{
    class IconBar : public Actor, public Drawable
    {
    public:
        IconBar(std::shared_ptr<IconBarSettings> settings, int maxValue = 10) :
            settings(settings), maxValue(maxValue)
        {
            texture = res::get<sf::Texture>(settings->iconPath);
        }

        virtual void draw(sf::RenderTarget& target, sf::RenderStates& states) override
        {
            sf::IntRect rect(
                (sf::Vector2i)settings->animationStrip.front(),
                (sf::Vector2i)settings->iconSize
            );
            sf::Sprite healthIcon(*texture, rect);
            target.draw(healthIcon, states);
        }

    private:
        std::shared_ptr<IconBarSettings> settings;
        std::shared_ptr<sf::Texture> texture;

        int maxValue;
        int currentValue;
    };
}