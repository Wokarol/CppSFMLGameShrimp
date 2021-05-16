#include <actors/debugPopup.h>
#include <assets/fonts.h>
#include <world.h>
#include <tweeners.h>
#include <memory>

auto color = 0xb03a2000;

DebugPopup::DebugPopup(std::string message)
{
	text.setString(message);
	font = fonts::get("RobotoMono");

	text.setFont(*font);
	text.setColor(sf::Color(color + 0xff));
	text.setCharacterSize(8 * 2);
	text.setScale(0.5f, 0.5f);

	text.setPosition(5.f, 5.f);
}

void DebugPopup::start()
{
	auto fadeOut = std::make_shared<LerpTweener<sf::Color>>(handle,
		[this]() { return text.getColor(); }, [this](auto v) { return text.setColor(v); },
		sf::Color(color), 1.f
		);

	auto myHandle = handle;
	fadeOut->getAfterKilled([myHandle]() {
		myHandle.destroy();
	});

	world::addTween(fadeOut);
}

void DebugPopup::draw(sf::RenderTarget& target)
{
	target.draw(text);
}
