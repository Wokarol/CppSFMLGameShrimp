# Surviving the Heat

A game made in C++ using SFML for Uni assigment

![Game screenshot](/screenshots/SurvivingTheHeat.png)

## File structure
```cs
├── game              // Game files
│   ├── assets        // Images, presets and so on
│   ├── include
│   └── src
├── lib               // External libraries and programs
│   ├── json
│   ├── ...
│   └── vendor
│       └── premake   // Build system
└── World.ldtk        // LDtk file containing levels
```
<br>

## Game files
- ` start.config ` - starting setting of the game like the level that's first loaded
- ` .json `/` .jsonc ` - Files containing information about the game like player speed
- ` .level ` - Generated files containing game levels extracted from LDtk (see below)
<br><br>

## LDtk - Level Designer Toolkit
![LDtk logo](https://ldtk.io/wp-content/uploads/2021/05/open-graph.png?width=400&height=121)

[You can get it here](https://ldtk.io/), this is a handy tool for simple level creation with suport for tilesets and entities.

This program outputs ` .ldtk ` file. Said file is in ` .json ` format and contains a lot of data not useful at runtime. We strip this additional data externally and output ` .level ` file which looks like so:
```jsonc
{
    "Title": "Intro",
    "Actors": {
        "Player": { "pos": [7.5, 9] },
        "Cacti": [
            { "pos": [13.5, 7] },
            { "pos": [10.5, 5] },
            // ... //
        ],
        "Tall_Cacti": [
            { "pos": [22.5, 15] },
            { "pos": [6.5, 15] },
            // ... //
        ]
    },
    "Tiles": [
        { "pos": [10, 1], "tPos": [2, 0] },
        { "pos": [11, 1], "tPos": [3, 0] },
        // ... //
    ],
    "Ground": [
        { "pos": [0, 0], "tPos": [0, 0] },
        { "pos": [1, 0], "tPos": [0, 0] },
        // ... //
    ]
}
```
This file is then directly read by the game
<br>

## Game assets
Game assets are stored in ` .jsonc ` or ` .json ` format and later parsed at runtime

#### Supported custom data types:
- (soon) *` sf::Rect ` - ` [x, y, width, heigh] `*
<br><br>

## Code architecture
Code is architected around code concept of "Actors" which are self contained objects in the game world, like tilemaps, player or enemies. All actors have to inherit from ` Actor ` class and can implement one of the classes like ` Drawable ` or ` Tickable `. Actors are grouped into "Groups" that can be, for example, unloaded at once.

Simple Actor might look like so:
```h
#include <actor.h>

class Crate : public Actor, public Drawable, public Hittable
{
public:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates& states) override;
    virtual float getSortingYPos() { return crateSprite.getPosition().y; }

    virtual void update(const GameClock& time) override;

private:
    sf::Sprite crateSprite;
}
```

Actors are created via ` createActor ` method like so:
```cpp
world::createActor<Crate>();
```
