#pragma once

#include <actor.h>

namespace wok
{
    class ScoreDisplay : public Actor, public Drawable, public Tickable
    {
    public:
        ScoreDisplay();

        virtual void update(const GameClock&) override;

        virtual void draw(sf::RenderTarget& target, sf::RenderStates& states) override;
        virtual int getSortingOrder() override { return 100000; }

    private:
        sf::Text text;
        std::shared_ptr<sf::Font> font;
    };
}