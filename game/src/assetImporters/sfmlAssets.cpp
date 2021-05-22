#include <resources.h>
#include <projectSettings.h>

template <>
static void wok::res::create(const std::string& name, std::shared_ptr<sf::Font>& asset)
{
    std::string fontPath = (std::stringstream()
        << "assets/" << project::fontsPath << name << ".ttf"
        ).str();

    if (!asset->loadFromFile(fontPath))
    {
        console::showImportant();
    }
}

template<>
static void wok::res::create(const std::string& name, std::shared_ptr<sf::Texture>& asset)
{
    std::string texturePath = (std::stringstream()
        << "assets/" << name << ".png"
        ).str();

    if (!asset->loadFromFile(texturePath))
    {
        console::showImportant();
    }
}
