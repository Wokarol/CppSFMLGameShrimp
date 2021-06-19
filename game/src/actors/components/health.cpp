#include <actors/components/health.h>
#include <actors/fracturedSprite.h>
#include <world.h>

void wok::Health::bindBar(ActorHandle<IconBar> bar)
{
    healthBar = bar;
}

void wok::Health::healUpTo(int newValue)
{
    value = newValue;

    if (healthBar.isValid()) healthBar->setValue(value);
}

void wok::Health::damage(Hittable::HitData data)
{
    value -= data.damage;

    if (healthBar.isValid()) healthBar->setValue(value);

    if (value <= 0)
    {
        onDeath(data);
    }
}
