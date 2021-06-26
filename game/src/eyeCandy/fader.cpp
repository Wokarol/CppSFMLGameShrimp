#include <fader.h>
#include <console.h>

bool wok::Fader::isFading()
{
    return state != FadeState::None;
}

bool wok::Fader::isFadingOut()
{
    return state == FadeState::FadeOut;
}

void wok::Fader::fade(std::function<void()> onFadeFull)
{
    if (isFading())
    {
        console::error("trying to fade twice, should not happen");
        return;
    }

    onFadeFullCallback = onFadeFull;
    t = 0.f;
    state = FadeState::FadeOut;
}

void wok::Fader::draw(sf::RenderTarget& render, sf::Vector2f screenSize)
{
    if (!isFading()) return;

    switch (state)
    {
    case FadeState::FadeOut:
        if (t > fadeOutDuration)
        {
            renderFade(render, screenSize, 1.f);
            state = FadeState::FadeIn;
            t = 0.f;
            onFadeFullCallback();
        }
        else
        {
            renderFade(render, screenSize, t / fadeOutDuration);
        }
        break;
    case FadeState::FadeIn:
        if (t > fadeInDuration)
        {
            renderFade(render, screenSize, 0.f);
            state = FadeState::None;
        }
        else
        {
            renderFade(render, screenSize, 1.f - (t / fadeInDuration));
        }
        break;
    }
}

void wok::Fader::renderFade(sf::RenderTarget& target, sf::Vector2f screenSize, float progress)
{
    sf::View previousView = target.getView();
    target.setView(sf::View({ 0.f, 0.f, screenSize.x, screenSize.y }));

    sf::RectangleShape fade(screenSize);
    fade.setFillColor(sf::Color(0.f, 0.f, 0.f, (sf::Uint8)(progress * 255)));

    target.draw(fade);

    target.setView(previousView);
}

void wok::Fader::update(const GameClock& time)
{
    if (!isFading()) return;

    t += time.delta;
}