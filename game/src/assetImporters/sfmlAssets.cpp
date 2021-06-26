#include <resources.h>
#include <projectSettings.h>

template <>
static void wok::res::create(const std::string& name, sf::Font& asset)
{
    std::string fontPath = (std::stringstream()
        << "assets/" << project::fontsPath << name << ".ttf"
        ).str();

    if (!asset.loadFromFile(fontPath))
    {
        console::showImportant();
    }
}

template<>
static void wok::res::create(const std::string& name, sf::Texture& asset)
{
    std::string texturePath = (std::stringstream()
        << "assets/" << name << ".png"
        ).str();

    if (!asset.loadFromFile(texturePath))
    {
        console::showImportant();
    }
}

template<>
static void wok::res::create(const std::string& name, sf::Shader& asset)
{
    std::string texturePath = (std::stringstream()
        << "assets/" << name << ".frag"
        ).str();

    if (!asset.loadFromFile(texturePath, sf::Shader::Type::Fragment))
    {
        console::showImportant();
    }
}
