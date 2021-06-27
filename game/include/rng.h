#pragma once
#include <random>

namespace randomizer
{
    namespace implementation
    {
        inline static std::mt19937 generator = std::mt19937(std::random_device()());
    }

    inline float get01()
    {
        auto distribution = std::uniform_real_distribution<float>(0.f, 1.f);
        return distribution(implementation::generator);
    }

    inline float getBetween(float a, float b)
    {
        // We flip the variables to satify uniform_real_distribution's assert
        if (a > b)
            return getBetween(b, a);

        auto distribution = std::uniform_real_distribution<float>(a, b);
        return distribution(implementation::generator);
    }

    // Max is exclusive while min is inclusive, that is (0, 5) will return only 0, 1, 2, 3
    inline float getBetween(int a, int b)
    {
        // We flip the variables to satify uniform_real_distribution's assert
        if (a > b)
            return getBetween(b, a);

        auto distribution = std::uniform_int_distribution<int>(a, b - 1);
        return distribution(implementation::generator);
    }
}