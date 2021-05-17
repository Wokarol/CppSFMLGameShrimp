#pragma once

#include <SFML/Graphics.hpp>

namespace csf
{
	class LineShape : public sf::Drawable
	{
		mutable sf::RectangleShape body;
		sf::Vector2f start;
		sf::Vector2f end;
		float thickness;

		mutable bool isDirty;

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
	};
}

