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
            currentValue = maxValue;
        }

        void setValue(int value)
        {
            currentValue = value;
        }

        virtual void draw(sf::RenderTarget& target, sf::RenderStates& states) override
        {
            sf::Vector2f iconOffset = sf::Vector2f(
                (float)settings->iconSize.x + settings->spacing,
                0
            );

            for (int i = 0; i < maxValue; i++)
            {
                sf::Vector2i iconPosOnTexture = currentValue > i
                    ? settings->animationStrip.front()
                    : settings->animationStrip.back();

                sf::IntRect rect(
                    iconPosOnTexture,
                    settings->iconSize
                );
                sf::Sprite healthIcon(*texture, rect);
                healthIcon.setPosition(settings->offset + iconOffset * (float)i);
                target.draw(healthIcon, states);
            }
        }
        virtual int getSortingOrder() override { return 100000; }

    private:
        std::shared_ptr<IconBarSettings> settings;
        std::shared_ptr<sf::Texture> texture;

        int maxValue;
        int currentValue;
    };
}