#pragma once

#include <actor.h>
#include <world.h>
#include <sstream>
#include <string>

namespace wok
{
    class DebugPopup : public Actor, public wok::Drawable, public Tickable
    {
        std::shared_ptr<sf::Font> font;
        sf::Text text;
        int lastPos = -1;

    public:
        DebugPopup(std::string message);

        virtual void start() override;
        virtual void update(const GameClock& time) override;

        virtual void draw(sf::RenderTarget& target, sf::RenderStates& states) override;
        virtual int getSortingOrder() override;

        template <class... Params>
        static void create(Params&&... params)
        {
            std::stringstream messageStream;
            ((messageStream << std::forward<Params>(params)), ...);
            std::string message = messageStream.str();

            std::string name = (std::stringstream() << message.substr(0, 10) << "...").str();

            world::createNamedActor<DebugPopup>(name, message);
        }

        virtual ~DebugPopup();
    };
}
