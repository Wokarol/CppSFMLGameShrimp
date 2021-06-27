#pragma once

#include <actor.h>

#include <assets/iconBarSettings.h>
#include <actors/components/twoStateFrameAnimator.h>

namespace wok
{
    class IconBar : public Actor, public Drawable, public Tickable
    {
    public:
        IconBar(std::shared_ptr<IconBarSettings> settings, int maxValue = 10);

        void setValue(int value);

        virtual void update(const GameClock& time) override;

        virtual void draw(sf::RenderTarget& target, sf::RenderStates& states) override;
        virtual int getSortingOrder() override { return 100000; }

    private:
        std::shared_ptr<IconBarSettings> settings;
        std::shared_ptr<sf::Texture> texture;

        std::vector<wok::TwoStateFrameAnimator<sf::Vector2i>> animators;
        std::vector<sf::Vector2i> frames;

        int maxValue;
        int currentValue;
    };
}