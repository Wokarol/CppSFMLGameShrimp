#include <actors/debugPopup.h>
#include <assets/fonts.h>
#include <world.h>
#include <tweeners.h>
#include <memory>

DebugPopup::DebugPopup(std::string message)
{
	text.setString(message);
	font = fonts::get("RobotoMono");

	text.setFont(*font);
	text.setColor(sf::Color(0xb03a20ff));
	text.setCharacterSize(10 * 2);
	text.setScale(0.5f, 0.5f);

	text.setPosition(10.f, 10.f);
}

void DebugPopup::start()
{
	auto fadeOut = std::make_shared<LerpTweener<sf::Color>>(handle,
		[this]() { return text.getColor(); }, [this](auto v) { return text.setColor(v); },
		sf::Color(0x00000000), 1.f
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
