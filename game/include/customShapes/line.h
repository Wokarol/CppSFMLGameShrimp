#pragma once

#include <SFML/Graphics.hpp>

namespace sf
{
	class LineShape : public Drawable
	{
		mutable RectangleShape body;
		Vector2f start;
		Vector2f end;
		float thickness;

		mutable bool isDirty;

	public:
		LineShape(float thickness = 10.f);

		void setStart(Vector2f);
		Vector2f getStart() const;
		void setEnd(Vector2f);
		Vector2f getEnd() const;

		void setThickness(float);
		float getThickness() const;

		void setColor(sf::Color);
		sf::Color getColor() const;

		virtual void draw(RenderTarget&, RenderStates) const override;

	private:
		void updateShape() const;
	};
}

