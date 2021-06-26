#include <fader.h>
#include <console.h>

#include <resources.h>
#include <rng.h>

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

    xDirectionFlipped = randomizer::get01() > 0.5f;
    yDirectionFlipped = randomizer::get01() > 0.5f;
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
            xDirectionFlipped = randomizer::get01() > 0.5f;
            yDirectionFlipped = randomizer::get01() > 0.5f;

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
    if (!shader)
        shader = res::get<sf::Shader>("shaders/screenFader");


    sf::View previousView = target.getView();
    target.setView(sf::View({ 0.f, 0.f, screenSize.x, screenSize.y }));

    sf::RenderStates states;
    states.shader = shader.get();

    sf::RectangleShape fade(screenSize);

    shader->setParameter("u_resolution", screenSize);
    shader->setParameter("u_t", progress);
    shader->setParameter("u_xDirectionFlipped", xDirectionFlipped);
    shader->setParameter("u_yDirectionFlipped", yDirectionFlipped);

    target.draw(fade, states);

    target.setView(previousView);
}

void wok::Fader::update(const GameClock& time)
{
    if (!isFading()) return;

    t += time.delta;
}