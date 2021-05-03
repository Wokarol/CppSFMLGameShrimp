#pragma once

#include <SFML/Graphics.hpp>
#include "FastNoiseLite.h"

class Shaker
{
    FastNoiseLite noiseR;// (48953);
    //noiseR.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2S);
    FastNoiseLite noiseX;// (4684684);
    //noiseX.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2S);
    FastNoiseLite noiseY;// (8228176);

public:
    Shaker(int seed)
    {
        noiseR = FastNoiseLite(seed);
        noiseX = FastNoiseLite(seed + 178998);
        noiseY = FastNoiseLite(seed + 127612);

        noiseR.SetNoiseType(FastNoiseLite::NoiseType::Perlin);
        noiseX.SetNoiseType(FastNoiseLite::NoiseType::Perlin);
        noiseY.SetNoiseType(FastNoiseLite::NoiseType::Perlin);
    }

    sf::Transform getShake(float t, float s)
    {
        auto m = sf::Transform::Identity;
        return m
            .translate(
                noiseX.GetNoise(t * 500, 0.0f) * 20.0f * s,
                noiseY.GetNoise(t * 500, 0.0f) * 20.0f * s)
            .rotate(noiseX.GetNoise(t * 500, 0.0f) * 4.0f * s);
    }
};