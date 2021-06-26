#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
#include <gameClock.h>

namespace wok
{
    class Fader
    {
        enum class FadeState { None, FadeOut, FadeIn };

    public:
        bool isFading();
        bool isFadingOut();

        void fade(std::function<void()> onFadeFull);
        void draw(sf::RenderTarget& render, sf::Vector2f screenSize);
        void update(const GameClock& time);

    private:
        void renderFade(sf::RenderTarget& target, sf::Vector2f screenSize, float progress);

    private:
        std::function<void()> onFadeFullCallback;

        FadeState state = FadeState::None;
        float t = 0.f;
        float fadeOutDuration = 2.f;
        float fadeInDuration = 2.f;

        bool xDirectionFlipped = false;
        bool yDirectionFlipped = false;

        std::shared_ptr<sf::Shader> shader;
    };
}