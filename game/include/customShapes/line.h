#pragma once

#include <SFML/Graphics.hpp>

namespace wok::csf
{
    class LineShape : public sf::Drawable
    {
    public:
        LineShape(float thickness = 10.f);

        void setStart(sf::Vector2f);
        sf::Vector2f getStart() const;
        void setEnd(sf::Vector2f);
        sf::Vector2f getEnd() const;

        void setThickness(float);
        float getThickness() const;

        void setColor(sf::Color);
        sf::Color getColor() const;

        virtual void draw(sf::RenderTarget&, sf::RenderStates) const override;

    private:
        void updateShape() const;

        mutable sf::RectangleShape body;
        mutable bool isDirty;

        sf::Vector2f start;
        sf::Vector2f end;
        float thickness;
    };
}
