#pragma once

#include <functional>

#include <actor.h>
#include <actors/iconBar.h>



namespace wok
{
    class Health
    {
    public:
        Health() = default;
        Health(ActorHandle<> owner, int startValue, std::function<void(Hittable::HitData)> onDeath = [](auto) {}) :
            owner(owner), value(startValue), onDeath(onDeath)
        {}

        void bindBar(ActorHandle<IconBar> healthBar);
        void healUpTo(int newValue);
        void damage(wok::Hittable::HitData data);
        void damage(int damageValue) { damage(Hittable::HitData(sf::Vector2f(), damageValue)); }

    private:
        ActorHandle<> owner;

        std::function<void(Hittable::HitData)> onDeath;
        int value = 0;
        ActorHandle<IconBar> healthBar;
    };
}
