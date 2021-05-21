#pragma once

#include <tweener.h>
#include <console.h>
#include <utils/mathUtils.h>

namespace wok
{
    template <class T>
    class SineTweener : public Tweener
    {
        std::function<void(T)> setter;
        float elapsedTime = 0;
        float speed;
        T min, max;

    public:
        SineTweener(ActorHandle<Actor> actor, std::function<void(T)> setter, T min, T max, float speed) :
            Tweener(actor),
            setter(setter),
            speed(speed),
            min(min), max(max)
        {}

        void addTimeOffset(float time)
        {
            elapsedTime += time;
        }

        virtual void tween(const GameClock& time) override
        {
            elapsedTime += time.delta * speed;

            float t = std::sin(elapsedTime) * 0.5f + 0.5f;

            setter(m::lerp(min, max, t));
        }
    };
}
