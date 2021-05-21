#include <actors/debugPopup.h>
#include <resources.h>
#include <world.h>
#include <tweeners.h>
#include <memory>
#include <limits>

std::vector<wok::DebugPopup*> popupsActive;


wok::DebugPopup::DebugPopup(std::string message)
{
    text.setString(message);
    font = res::get<sf::Font>(fontName);

    text.setFont(*font);
    text.setFillColor(sf::Color(fontColor | 0xff));

    // We do scaling to make text sharper
    text.setCharacterSize(8 * 2);
    text.setScale(0.5f, 0.5f);

    // We store all popups in static vector to use them to calculate this one's position
    popupsActive.push_back(this);
}

void wok::DebugPopup::start()
{
    auto fadeOut = std::make_shared<LerpTweener<sf::Color>>(handle,
        [this]() { return text.getFillColor(); }, [this](auto v) { return text.setFillColor(v); },
        sf::Color(fontColor & ~0xff), 2.f
        );

    auto myHandle = handle;
    fadeOut->setAfterKilled([myHandle]() {
        myHandle.destroy();
        });

    world::addTween(fadeOut);
}

void wok::DebugPopup::update([[maybe_unused]] const GameClock& time)
{
    auto it = std::find(popupsActive.begin(), popupsActive.end(), this);
    int pos = (int)(it - popupsActive.begin());

    if (lastPos == -1)
    {
        text.setPosition(5.f, 5.f + pos * 10);
    }
    else if (lastPos != pos)
    {
        updatePositionTo(sf::Vector2f(5.f, 5.f + pos * 10));
    }
    lastPos = pos;
}

void wok::DebugPopup::updatePositionTo(sf::Vector2f pos)
{
    auto slide = std::make_shared<LerpTweener<sf::Vector2f>>(handle,
        [this]() { return text.getPosition(); }, [this](auto v) { text.setPosition(v); },
        pos, 0.5f);
    world::addTween(slide);
}

void wok::DebugPopup::draw(sf::RenderTarget& target, sf::RenderStates& states)
{
    target.draw(text, states);
}

int wok::DebugPopup::getSortingOrder()
{
    return std::numeric_limits<int>::max();
}

wok::DebugPopup::~DebugPopup()
{
    popupsActive.erase(std::find(popupsActive.begin(), popupsActive.end(), this));
}
